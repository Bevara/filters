#include <gpac/filters.h>
#include <gpac/avparse.h>

typedef struct
{
	u32 codecid;
	GF_FilterPid *ipid, *opid;
	u32 width, height, pixel_format, BPP;
} GF_PNGDecCtx;

static GF_Err pngdec_configure_pid(GF_Filter *filter, GF_FilterPid *pid, Bool is_remove)
{
	const GF_PropertyValue *prop;
	GF_PNGDecCtx *ctx = (GF_PNGDecCtx *)gf_filter_get_udta(filter);

	// disconnect of src pid (not yet supported)
	if (is_remove)
	{
		if (ctx->opid)
		{
			gf_filter_pid_remove(ctx->opid);
			ctx->opid = NULL;
		}
		ctx->ipid = NULL;
		return GF_OK;
	}
	if (!gf_filter_pid_check_caps(pid))
		return GF_NOT_SUPPORTED;

	prop = gf_filter_pid_get_property(pid, GF_PROP_PID_CODECID);
	if (!prop)
		return GF_NOT_SUPPORTED;
	ctx->codecid = prop->value.uint;
	ctx->ipid = pid;

	if (!ctx->opid)
	{
		ctx->opid = gf_filter_pid_new(filter);
	}
	// copy properties at init or reconfig
	gf_filter_pid_copy_properties(ctx->opid, ctx->ipid);
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_CODECID, &PROP_UINT(GF_CODECID_RAW));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(GF_PIXEL_RGB));

	if (ctx->codecid == GF_CODECID_PNG)
	{
		gf_filter_set_name(filter, "pngdec:libpng");
	}
	return GF_OK;
}

static GF_Err pngdec_process(GF_Filter *filter)
{
	GF_Err e;
	GF_FilterPacket *pck;
	u8 *data, *output;
	u32 size;
	GF_PNGDecCtx *ctx = (GF_PNGDecCtx *)gf_filter_get_udta(filter);

	pck = gf_filter_pid_get_packet(ctx->ipid);
	if (!pck)
	{
		if (gf_filter_pid_is_eos(ctx->ipid))
		{
			gf_filter_pid_set_eos(ctx->opid);
			return GF_EOS;
		}
		return GF_OK;
	}
	data = (u8 *)gf_filter_pck_get_data(pck, &size);

	GF_FilterPacket *dst_pck;
	u32 out_size = 0;
	u32 w = ctx->width;
	u32 h = ctx->height;
	u32 pf = ctx->pixel_format;

	e = gf_img_png_dec(data, size, &ctx->width, &ctx->height, &ctx->pixel_format, NULL, &out_size);

	if (e != GF_BUFFER_TOO_SMALL)
	{
		gf_filter_pid_drop_packet(ctx->ipid);
		return e;
	}
	if ((w != ctx->width) || (h != ctx->height) || (pf != ctx->pixel_format))
	{
		switch (ctx->pixel_format)
		{
		case GF_PIXEL_GREYSCALE:
			ctx->BPP = 1;
			break;
		case GF_PIXEL_RGB:
			ctx->BPP = 3;
			break;
		case GF_PIXEL_RGBA:
			ctx->BPP = 4;
			break;
		}
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_WIDTH, &PROP_UINT(ctx->width));
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_HEIGHT, &PROP_UINT(ctx->height));
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(ctx->pixel_format));
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_STRIDE, &PROP_UINT(ctx->BPP * ctx->width));
	}
	dst_pck = gf_filter_pck_new_alloc(ctx->opid, out_size, &output);
	if (!dst_pck)
		return GF_OUT_OF_MEM;

	e = gf_img_png_dec(data, size, &ctx->width, &ctx->height, &ctx->pixel_format, output, &out_size);

	if (e)
	{
		gf_filter_pck_discard(dst_pck);
	}
	else
	{
		gf_filter_pck_merge_properties(pck, dst_pck);
		gf_filter_pck_set_dependency_flags(dst_pck, 0);
		gf_filter_pck_send(dst_pck);
	}
	gf_filter_pid_drop_packet(ctx->ipid);
	return GF_OK;
}

static const GF_FilterCapability ImgDecCaps[] =
	{
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_BOOL(GF_CAPS_INPUT_EXCLUDED, GF_PROP_PID_UNFRAMED, GF_TRUE),
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_CODECID, GF_CODECID_PNG),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_CODECID, GF_CODECID_RAW),
};

GF_FilterRegister ImgPngRegister = {
	.name = "pngdec",
	GF_FS_SET_DESCRIPTION("PNG decoder")
		GF_FS_SET_HELP("This filter decodes PNG images.")
			.private_size = sizeof(GF_PNGDecCtx),
	.priority = 1,
	SETCAPS(ImgDecCaps),
	.configure_pid = pngdec_configure_pid,
	.process = pngdec_process,
};

const GF_FilterRegister *dynCall_pngdec_register(GF_FilterSession *session)
{
	return &ImgPngRegister;
}
