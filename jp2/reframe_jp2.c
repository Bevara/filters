
#include <gpac/filters.h>
#include <gpac/constants.h>
#include <gpac/avparse.h>

#ifdef GPAC_HAS_JP2

// we MUST set OPJ_STATIC before including openjpeg.h
#if !defined(__GNUC__) && (defined(_WIN32_WCE) || defined(WIN32))
#define OPJ_STATIC
#endif

#include <gpac/constants.h>
#include <gpac/bitstream.h>
#include <gpac/isomedia.h>

#include <openjpeg.h>

#ifdef OPJ_PROFILE_NONE
#define OPENJP2 1

#if !defined(__GNUC__) && (defined(_WIN32_WCE) || defined(WIN32))
#if defined(_DEBUG)
#pragma comment(lib, "libopenjp2d")
#else
#pragma comment(lib, "libopenjp2")
#endif
#endif

#else

#define OPENJP2 0

#if !defined(__GNUC__) && (defined(_WIN32_WCE) || defined(WIN32))
#if defined(_DEBUG)
#pragma comment(lib, "LibOpenJPEGd")
#else
#pragma comment(lib, "LibOpenJPEG")
#endif
#endif

#endif
#endif

#if OPENJP2
typedef struct
{
	char *data;
	u32 len, pos;
} OJP2Frame;

static OPJ_SIZE_T j2kdec_stream_read(void *out_buffer, OPJ_SIZE_T nb_bytes, void *user_data)
{
	OJP2Frame *frame = user_data;
	u32 remain;
	if (frame->pos == frame->len)
		return (OPJ_SIZE_T)-1;
	remain = frame->len - frame->pos;
	if (nb_bytes > remain)
		nb_bytes = remain;
	memcpy(out_buffer, frame->data + frame->pos, nb_bytes);
	frame->pos += (u32)nb_bytes;
	return nb_bytes;
}

static OPJ_OFF_T j2kdec_stream_skip(OPJ_OFF_T nb_bytes, void *user_data)
{
	OJP2Frame *frame = user_data;
	if (!user_data)
		return 0;

	if (nb_bytes < 0)
	{
		if (frame->pos == 0)
			return (OPJ_SIZE_T)-1;
		if (nb_bytes + (s32)frame->pos < 0)
		{
			nb_bytes = -frame->pos;
		}
	}
	else
	{
		u32 remain;
		if (frame->pos == frame->len)
		{
			return (OPJ_SIZE_T)-1;
		}
		remain = frame->len - frame->pos;
		if (nb_bytes > remain)
		{
			nb_bytes = remain;
		}
	}
	frame->pos += (u32)nb_bytes;
	return nb_bytes;
}

static OPJ_BOOL j2kdec_stream_seek(OPJ_OFF_T nb_bytes, void *user_data)
{
	OJP2Frame *frame = user_data;
	if (nb_bytes < 0 || nb_bytes > frame->pos)
		return OPJ_FALSE;
	frame->pos = (u32)nb_bytes;
	return OPJ_TRUE;
}
#endif

typedef struct
{
	// options
	GF_Fraction fps;

	// only one input pid declared
	GF_FilterPid *ipid;
	// only one output pid declared
	GF_FilterPid *opid;
	u32 src_timescale;
	Bool is_bmp;
	Bool owns_timescale;
	u32 codec_id;

	Bool initial_play_done;
	Bool is_playing;
} GF_ReframeJp2Ctx;

GF_Err jp2_configure_pid(GF_Filter *filter, GF_FilterPid *pid, Bool is_remove)
{
	GF_ReframeJp2Ctx *ctx = gf_filter_get_udta(filter);
	const GF_PropertyValue *p;

	if (is_remove)
	{
		ctx->ipid = NULL;
		return GF_OK;
	}

	if (!gf_filter_pid_check_caps(pid))
		return GF_NOT_SUPPORTED;

	gf_filter_pid_set_framing_mode(pid, GF_TRUE);
	ctx->ipid = pid;
	// force retest of codecid
	ctx->codec_id = 0;

	p = gf_filter_pid_get_property(pid, GF_PROP_PID_TIMESCALE);
	if (p)
		ctx->src_timescale = p->value.uint;

	if (ctx->src_timescale && !ctx->opid)
	{
		ctx->opid = gf_filter_pid_new(filter);
		gf_filter_pid_copy_properties(ctx->opid, ctx->ipid);
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_UNFRAMED, NULL);
	}
	ctx->is_playing = GF_TRUE;
	return GF_OK;
}

Bool jp2_process_event(GF_Filter *filter, const GF_FilterEvent *evt)
{
	GF_FilterEvent fevt;
	GF_ReframeJp2Ctx *ctx = gf_filter_get_udta(filter);
	if (evt->base.on_pid != ctx->opid)
		return GF_TRUE;
	switch (evt->base.type)
	{
	case GF_FEVT_PLAY:
		if (ctx->is_playing)
		{
			return GF_TRUE;
		}

		ctx->is_playing = GF_TRUE;
		if (!ctx->initial_play_done)
		{
			ctx->initial_play_done = GF_TRUE;
			return GF_TRUE;
		}

		GF_FEVT_INIT(fevt, GF_FEVT_SOURCE_SEEK, ctx->ipid);
		fevt.seek.start_offset = 0;
		gf_filter_pid_send_event(ctx->ipid, &fevt);
		return GF_TRUE;
	case GF_FEVT_STOP:
		ctx->is_playing = GF_FALSE;
		return GF_FALSE;
	default:
		break;
	}
	// cancel all events
	return GF_TRUE;
}

GF_Err jp2_process(GF_Filter *filter)
{
	GF_ReframeJp2Ctx *ctx = gf_filter_get_udta(filter);
	GF_FilterPacket *pck, *dst_pck;
	GF_Err e;
	u8 *data, *output;
	u32 size, w = 0, h = 0, pf = 0;
	u8 *pix;
	u32 i, j, irow, in_stride, out_stride;
	GF_BitStream *bs;

	pck = gf_filter_pid_get_packet(ctx->ipid);
	if (!pck)
	{
		if (gf_filter_pid_is_eos(ctx->ipid))
		{
			if (ctx->opid)
				gf_filter_pid_set_eos(ctx->opid);
			ctx->is_playing = GF_FALSE;
			return GF_EOS;
		}
		return GF_OK;
	}
	data = (u8 *)gf_filter_pck_get_data(pck, &size);

	if (!ctx->opid || !ctx->codec_id)
	{
		u32 dsi_size;
		u8 *dsi = NULL;

		const char *ext, *mime;
		const GF_PropertyValue *prop;
		u32 codecid = 0;

		bs = gf_bs_new(data, size, GF_BITSTREAM_READ);
		gf_img_parse(bs, &codecid, &w, &h, &dsi, &dsi_size);
		gf_bs_del(bs);

#ifdef GPAC_HAS_JP2
		if (!dsi)
		{
			// This is a j2k file FIXME : this could be handled by the decoder
			opj_codec_t *codec;
			opj_stream_t *stream;
			opj_dparameters_t parameters;
			OJP2Frame ojp2frame;
			opj_image_t *image = NULL;

			opj_set_default_decoder_parameters(&parameters);
			codec = opj_create_decompress(OPJ_CODEC_J2K);
			opj_setup_decoder(codec, &parameters);
			stream = opj_stream_default_create(OPJ_STREAM_READ);
			opj_stream_set_read_function(stream, j2kdec_stream_read);
			opj_stream_set_skip_function(stream, j2kdec_stream_skip);
			opj_stream_set_seek_function(stream, j2kdec_stream_seek);
			ojp2frame.data = data;
			ojp2frame.len = size;
			ojp2frame.pos = 0;

			opj_stream_set_user_data(stream, &ojp2frame, NULL);
			opj_stream_set_user_data_length(stream, ojp2frame.len);
			opj_read_header(stream, codec, &image);

			switch (image->numcomps)
			{
			case 1:
				pf = GF_PIXEL_GREYSCALE;
				break;
			case 2:
				pf = GF_PIXEL_ALPHAGREY;
				break;
			case 3:
				pf = GF_PIXEL_RGB;
				break;
			case 4:
				pf = GF_PIXEL_RGBA;
				break;
			}

			opj_image_destroy(image);
			opj_stream_destroy(stream);
			opj_destroy_codec(codec);
		}
#endif

	prop = gf_filter_pid_get_property(ctx->ipid, GF_PROP_PID_FILE_EXT);
	ext = (prop && prop->value.string) ? prop->value.string : "";
	prop = gf_filter_pid_get_property(ctx->ipid, GF_PROP_PID_MIME);
	mime = (prop && prop->value.string) ? prop->value.string : "";

	if (!codecid)
	{
		if (!stricmp(ext, "jp2") || !stricmp(ext, "j2k") || !strcmp(mime, "image/jp2"))
		{
			codecid = GF_CODECID_J2K;
		}
	}
	if (!codecid)
	{
		gf_filter_pid_drop_packet(ctx->ipid);
		return GF_NOT_SUPPORTED;
	}
	ctx->codec_id = codecid;
	ctx->opid = gf_filter_pid_new(filter);
	if (!ctx->opid)
	{
		gf_filter_pid_drop_packet(ctx->ipid);
		return GF_SERVICE_ERROR;
	}
	if (!ctx->fps.num || !ctx->fps.den)
	{
		ctx->fps.num = 1000;
		ctx->fps.den = 1000;
	}
	// we don't have input reconfig for now
	gf_filter_pid_copy_properties(ctx->opid, ctx->ipid);
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_STREAM_TYPE, &PROP_UINT(GF_STREAM_VISUAL));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_CODECID, &PROP_UINT(codecid));
	if (pf)
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(pf));
	if (w)
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_WIDTH, &PROP_UINT(w));
	if (h)
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_HEIGHT, &PROP_UINT(h));

	if (dsi)
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_DECODER_CONFIG, &PROP_DATA_NO_COPY(dsi, dsi_size));

	if (!gf_filter_pid_get_property(ctx->ipid, GF_PROP_PID_TIMESCALE))
	{
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_TIMESCALE, &PROP_UINT(ctx->fps.num));
		ctx->owns_timescale = GF_TRUE;
	}

	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_NB_FRAMES, &PROP_UINT(1));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PLAYBACK_MODE, &PROP_UINT(GF_PLAYBACK_MODE_FASTFORWARD));

	if (ext || mime)
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_CAN_DATAREF, &PROP_BOOL(GF_TRUE));
}

e = GF_OK;
u32 start_offset = 0;
if (ctx->codec_id == GF_CODECID_J2K)
{

	if (size < 8)
	{
		gf_filter_pid_drop_packet(ctx->ipid);
		return GF_NON_COMPLIANT_BITSTREAM;
	}

	if ((data[4] == 'j') && (data[5] == 'P') && (data[6] == ' ') && (data[7] == ' '))
	{
		bs = gf_bs_new(data, size, GF_BITSTREAM_READ);
		while (gf_bs_available(bs))
		{
			u32 bsize = gf_bs_read_u32(bs);
			u32 btype = gf_bs_read_u32(bs);
			if (btype == GF_4CC('j', 'p', '2', 'c'))
			{
				start_offset = (u32)gf_bs_get_position(bs) - 8;
				break;
			}
			gf_bs_skip_bytes(bs, bsize - 8);
		}
		gf_bs_del(bs);
		if (start_offset >= size)
		{
			gf_filter_pid_drop_packet(ctx->ipid);
			return GF_NON_COMPLIANT_BITSTREAM;
		}
	}
}
dst_pck = gf_filter_pck_new_ref(ctx->opid, start_offset, size - start_offset, pck);
if (!dst_pck)
	return GF_OUT_OF_MEM;

gf_filter_pck_merge_properties(pck, dst_pck);
if (ctx->owns_timescale)
{
	gf_filter_pck_set_cts(dst_pck, 0);
	gf_filter_pck_set_sap(dst_pck, GF_FILTER_SAP_1);
	gf_filter_pck_set_duration(dst_pck, ctx->fps.den);
}
gf_filter_pck_send(dst_pck);
gf_filter_pid_drop_packet(ctx->ipid);
return e;
}

#include <gpac/internal/isomedia_dev.h>

static const char *jp2_probe_data(const u8 *data, u32 size, GF_FilterProbeScore *score)
{
	GF_BitStream *bs = gf_bs_new(data, size, GF_BITSTREAM_READ);
	u32 bsize = gf_bs_read_u32(bs);
	u32 btype = gf_bs_read_u32(bs);
	if ((bsize == 12) && ((btype == GF_ISOM_BOX_TYPE_JP) || (btype == GF_ISOM_BOX_TYPE_JP2H)))
	{
		if (btype == GF_ISOM_BOX_TYPE_JP2H)
		{
			*score = GF_FPROBE_FORCE;
			gf_bs_del(bs);
			return "image/jp2";
		}
		btype = gf_bs_read_u32(bs);
		if (btype == 0x0D0A870A)
		{
			*score = GF_FPROBE_FORCE;
			gf_bs_del(bs);
			return "image/jp2";
		}
	}else if (bsize == 0xFF4FFF51 ) {
			/* Start-of-codestream (SOC) + Image size (SIZ) magic number */
			*score = GF_FPROBE_FORCE;
			gf_bs_del(bs);
			return "image/jp2";
	}
	gf_bs_del(bs);
	return NULL;
}
static const GF_FilterCapability ReframeJp2Caps[] =
	{
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_FILE),
		CAP_STRING(GF_CAPS_INPUT, GF_PROP_PID_FILE_EXT, "jp2|j2k"),
		CAP_STRING(GF_CAPS_INPUT, GF_PROP_PID_MIME, "image/jp2"),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_CODECID, GF_CODECID_J2K)
		//	CAP_BOOL(GF_CAPS_OUTPUT_EXCLUDED, GF_PROP_PID_UNFRAMED, GF_TRUE),
};

#define OFFS(_n) #_n, offsetof(GF_ReframeJp2Ctx, _n)
static const GF_FilterArgs ReframeJp2Args[] =
	{
		{OFFS(fps), "import frame rate (0 default to 1 Hz)", GF_PROP_FRACTION, "0/1000", NULL, GF_FS_ARG_HINT_HIDE},
		{0}};

GF_FilterRegister ReframeJP2Register = {
	.name = "rfjp2",
	GF_FS_SET_DESCRIPTION("J2K reframer")
		GF_FS_SET_HELP("This filter parses J2K files/data and outputs corresponding visual PID and frames.")
			.private_size = sizeof(GF_ReframeJp2Ctx),
	.args = ReframeJp2Args,
	SETCAPS(ReframeJp2Caps),
	.configure_pid = jp2_configure_pid,
	.probe_data = jp2_probe_data,
	.process = jp2_process,
	.process_event = jp2_process_event};

const GF_FilterRegister *dynCall_jp2_reframe_register(GF_FilterSession *session)
{
	return &ReframeJP2Register;
}
