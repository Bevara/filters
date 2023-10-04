#include <gpac/filters.h>
#include <gpac/avparse.h>

typedef struct
{
	u32 codecid;
	GF_FilterPid *ipid, *opid;
	u32 width, height, pixel_format, BPP;
	u32 ofmt;
} GF_JPEGDecCtx;

static GF_Err jpegdec_configure_pid(GF_Filter *filter, GF_FilterPid *pid, Bool is_remove)
{
	const GF_PropertyValue *prop;
	GF_JPEGDecCtx *ctx = (GF_JPEGDecCtx *)gf_filter_get_udta(filter);

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
	
	if (!ctx->ofmt) {
        ctx->ofmt = GF_PIXEL_RGB;
    }
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(ctx->ofmt));

	if (ctx->codecid == GF_CODECID_JPEG)
	{
		gf_filter_set_name(filter, "jpegdec");
	}

	return GF_OK;
}

static GF_Err jpegdec_reconfigure_output(GF_Filter *filter, GF_FilterPid *pid)
{
	const GF_PropertyValue *p;
	GF_JPEGDecCtx *ctx = gf_filter_get_udta(filter);
	if (ctx->opid != pid) return GF_BAD_PARAM;

	p = gf_filter_pid_caps_query(pid, GF_PROP_PID_PIXFMT);
	if (p) ctx->ofmt = p->value.uint;
	return jpegdec_configure_pid(filter, ctx->ipid, GF_FALSE);
}


static void convert_to_rgba(char* rgba, const char* rgb, const int count) {
    for(int i=0; i<count; ++i) {
        for(int j=0; j<3; ++j) {
            rgba[j] = rgb[j];
        }
		rgba[3] = 255;
        rgba += 4;
        rgb  += 3;
    }
}

static GF_Err jpegdec_process(GF_Filter *filter)
{
	GF_Err e;
	GF_FilterPacket *pck;
	u8 *data, *output;
	u32 size;
	GF_JPEGDecCtx *ctx = (GF_JPEGDecCtx *)gf_filter_get_udta(filter);
	Bool convert = GF_FALSE;

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
	u32 pf = ctx->ofmt;

	if (ctx->codecid == GF_CODECID_JPEG)
	{
		e = gf_img_jpeg_dec(data, size, &ctx->width, &ctx->height, &ctx->pixel_format, NULL, &out_size, ctx->BPP);
	}

	if (e != GF_BUFFER_TOO_SMALL)
	{
		gf_filter_pid_drop_packet(ctx->ipid);
		return e;
	}
	if ((w != ctx->width) || (h != ctx->height) || (pf != ctx->pixel_format))
	{
		switch (pf)
		{
		case GF_PIXEL_GREYSCALE:
			ctx->BPP = 1;
			break;
		case GF_PIXEL_RGB:
			ctx->BPP = 3;
			break;
		case GF_PIXEL_RGBA:
			ctx->BPP = 4;
			out_size = ctx->BPP * ctx->width * ctx->height;
			convert = GF_TRUE;
			break;
		}
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_WIDTH, &PROP_UINT(ctx->width));
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_HEIGHT, &PROP_UINT(ctx->height));
		//gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(pf));
		gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_STRIDE, &PROP_UINT(ctx->BPP * ctx->width));
	}
	dst_pck = gf_filter_pck_new_alloc(ctx->opid, out_size, &output);
	if (!dst_pck)
		return GF_OUT_OF_MEM;

	if (ctx->codecid == GF_CODECID_JPEG)
	{
		if(convert){
			u32 tmp_out_size = ctx->width * ctx->height * 3;
			u32* src = (u32*)gf_malloc(ctx->width * ctx->height * 3);
			e = gf_img_jpeg_dec(data, size, &ctx->width, &ctx->height, &ctx->pixel_format, src, &tmp_out_size, 3);
			convert_to_rgba(output, src, ctx->width * ctx->height );
			gf_free(src);
		}else{
			e = gf_img_jpeg_dec(data, size, &ctx->width, &ctx->height, &ctx->pixel_format, output, &out_size, ctx->BPP);
		}
	}



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

static const GF_FilterCapability JpegDecCaps[] =
	{
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_BOOL(GF_CAPS_INPUT_EXCLUDED, GF_PROP_PID_UNFRAMED, GF_TRUE),
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_CODECID, GF_CODECID_JPEG),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_CODECID, GF_CODECID_RAW),
};

GF_FilterRegister JpegDecRegister = {
	.name = "jpegdec",
	GF_FS_SET_DESCRIPTION("JPG decoder")
		GF_FS_SET_HELP("This filter decodes JPEG images.")
			.private_size = sizeof(GF_JPEGDecCtx),
	.priority = 1,
	SETCAPS(JpegDecCaps),
	.configure_pid = jpegdec_configure_pid,
	.reconfigure_output = jpegdec_reconfigure_output,
	.process = jpegdec_process,
};

const GF_FilterRegister *dynCall_jpegdec_register(GF_FilterSession *session)
{
	return &JpegDecRegister;
}
