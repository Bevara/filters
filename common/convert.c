#include <gpac/filters.h>

void convert_rgb_to_rgba(char* rgba, const char* rgb, u32 count) {
    for(int i=0; i<count; ++i) {
        for(int j=0; j<3; ++j) {
            rgba[j] = rgb[j];
        }
		rgba[3] = 255;
        rgba += 4;
        rgb  += 3;
    }
}

void convert_rgba_to_rgb(char* rgb, const char* rgba, u32 count) {
    for(int i=0; i<count; ++i) {
        for(int j=0; j<3; ++j) {
            rgb[j] = rgba[j];
        }
        rgba += 4;
        rgb  += 3;
    }
}

GF_Err convert(char* out, u32 out_format, const char* in, u32 in_format, u32 count){
	if (in_format == GF_PIXEL_RGB && out_format == GF_PIXEL_RGBA ){
		convert_rgb_to_rgba(out, in, count);
	}else if (in_format == GF_PIXEL_RGBA && out_format == GF_PIXEL_RGB ){
		convert_rgba_to_rgb(out, in, count);
	}else {
		return GF_NOT_SUPPORTED;
	}

	return GF_OK;
}