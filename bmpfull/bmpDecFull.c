/* BEVARA
**
** Adapted from the qdbmp code; see QDBMP license information below
**
** This is a starting point for full BMP support. Currently, this reads in the palette and handles
** 32, 24, and 8bpp (non-indexed) to RGBA format. Later implementations should handle other, less popular, BMP versions.
**
**
*/

/**************************************************************

	QDBMP - Quick n' Dirty BMP

	v1.0.0 - 2007-04-07
	http://qdbmp.sourceforge.net


	The library supports the following BMP variants:
	1. Uncompressed 32 BPP (alpha values are ignored)
	2. Uncompressed 24 BPP
	3. Uncompressed 8 BPP (indexed color)

	QDBMP is free and open source software, distributed
	under the MIT licence.

	Copyright (c) 2007 Chai Braudo (braudo@users.sourceforge.net)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

**************************************************************/
#include <gpac/filters.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * For convenience, the short BMP header in the original qdbmp file is rolled into this .c file
 */
/* Type definitions */
#ifndef UINT
#define UINT unsigned long int
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

/* Bitmap header */
struct BMP_Header
{
	USHORT Magic;		  /* Magic identifier: "BM" */
	UINT FileSize;		  /* Size of the BMP file in bytes */
	USHORT Reserved1;	  /* Reserved */
	USHORT Reserved2;	  /* Reserved */
	UINT DataOffset;	  /* Offset of image data relative to the file's start */
	UINT HeaderSize;	  /* Size of the header in bytes */
	UINT Width;			  /* Bitmap's width */
	UINT Height;		  /* Bitmap's height */
	USHORT Planes;		  /* Number of color planes in the bitmap */
	USHORT BitsPerPixel;  /* Number of bits per pixel */
	UINT CompressionType; /* Compression type */
	UINT ImageDataSize;	  /* Size of uncompressed image's data */
	UINT HPixelsPerMeter; /* Horizontal resolution (pixels per meter) */
	UINT VPixelsPerMeter; /* Vertical resolution (pixels per meter) */
	UINT ColorsUsed;	  /* Number of color indexes in the color table that are actually used by the bitmap */
	UINT ColorsRequired;  /* Number of color indexes that are required for displaying the bitmap */
};

/* Private data structure */
struct BMP_struct
{
	struct BMP_Header Header;
	UCHAR *Palette;
	UCHAR *Data;
};

/* Size of the palette data for 8 BPP bitmaps */
#define BMP_PALETTE_SIZE (256 * 4)

/* our BMP */
extern struct BMP_struct *bmp;

/*********************************** Public methods **********************************/

/* Construction/destruction */
int EMSCRIPTEN_KEEPALIVE BMP_Create(UINT width, UINT height, USHORT depth);
void EMSCRIPTEN_KEEPALIVE BMP_Free();

/* I/O */
int EMSCRIPTEN_KEEPALIVE BMP_Decode(const char *bmp_data, const int size);
unsigned int *EMSCRIPTEN_KEEPALIVE BMP_GetImage(void);

/* Meta info */
int EMSCRIPTEN_KEEPALIVE BMP_GetWidth();
int EMSCRIPTEN_KEEPALIVE BMP_GetHeight();
int EMSCRIPTEN_KEEPALIVE BMP_GetDepth();

/*********************************** Forward declarations **********************************/
GF_Err ReadHeader(unsigned char *bmp_data, const int size);
GF_Err ReadUINT(UINT *x, unsigned char *bmp_data, const int size);
GF_Err ReadUSHORT(USHORT *x, unsigned char *bmp_data, const int size);

/* our BMP - globals go here */
struct BMP_struct *bmp;
long dataInd;

typedef struct
{
	GF_FilterPid *ipid, *opid;
} GF_BMPFullCtx;

/*********************************** Public methods **********************************/

static GF_Err BMPFull_configure_pid(GF_Filter *filter, GF_FilterPid *pid, Bool is_remove)
{
	const GF_PropertyValue *prop;
	GF_BMPFullCtx *ctx = (GF_BMPFullCtx *)gf_filter_get_udta(filter);

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

	ctx->ipid = pid;

	if (!ctx->opid)
	{
		ctx->opid = gf_filter_pid_new(filter);
	}
	gf_filter_pid_set_framing_mode(pid, GF_TRUE);

	// copy properties at init or reconfig
	gf_filter_pid_copy_properties(ctx->opid, ctx->ipid);
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_CODECID, &PROP_UINT(GF_CODECID_RAW));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_STREAM_TYPE, &PROP_UINT(GF_STREAM_VISUAL));

	gf_filter_set_name(filter, "BMPFull");

	return GF_OK;
}

static const GF_FilterCapability BMPFullCaps[] =
	{
		CAP_UINT(GF_CAPS_INPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_FILE),
		CAP_STRING(GF_CAPS_INPUT, GF_PROP_PID_FILE_EXT, "bmp"),
		CAP_STRING(GF_CAPS_INPUT, GF_PROP_PID_MIME, "image/bmp"),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_STREAM_TYPE, GF_STREAM_VISUAL),
		CAP_UINT(GF_CAPS_OUTPUT, GF_PROP_PID_CODECID, GF_CODECID_RAW),
};

/**************************************************************
	Reads the specified BMP image file.
**************************************************************/
GF_Err EMSCRIPTEN_KEEPALIVE bmpFullDec_process(GF_Filter *filter)
{

	u32 i, j, size;

	u8 *data, *buffer;

	GF_BMPFullCtx *ctx = gf_filter_get_udta(filter);

	GF_FilterPacket *pck, *pck_dst;
	u8 bpp = 4;

	pck = gf_filter_pid_get_packet(ctx->ipid);
	if (!pck)
	{
		if (gf_filter_pid_is_eos(ctx->ipid))
		{
			if (ctx->opid)
				gf_filter_pid_set_eos(ctx->opid);
			return GF_EOS;
		}
		return GF_OK;
	}
	data = (unsigned char *)gf_filter_pck_get_data(pck, &size);

	// this is the decode section

	/* Allocate */
	bmp = (struct BMP_struct *)malloc(sizeof(struct BMP_struct));
	if (bmp == NULL)
	{
		return GF_OUT_OF_MEM;
	}

	dataInd = 0;
	/* Read header */
	if ((ReadHeader(data, size) != GF_OK) || (bmp->Header.Magic != 0x4D42))
	{

		free(bmp);
		return GF_NOT_SUPPORTED;
	}

	/* Verify that the bitmap variant is supported */
	if ((bmp->Header.BitsPerPixel != 32 && bmp->Header.BitsPerPixel != 24 && bmp->Header.BitsPerPixel != 8) || bmp->Header.CompressionType != 0 || bmp->Header.HeaderSize != 40)
	{
		free(bmp);
		return GF_NOT_SUPPORTED;
	}

	/* Allocate and read palette */
	if (bmp->Header.BitsPerPixel == 8)
	{
		bmp->Palette = (UCHAR *)malloc(BMP_PALETTE_SIZE * sizeof(UCHAR));
		if (bmp->Palette == NULL)
		{
			free(bmp);
			return GF_OUT_OF_MEM;
		}

		if (dataInd + BMP_PALETTE_SIZE > size)
		{
			free(bmp->Palette);
			free(bmp);
			return GF_CORRUPTED_DATA;
		}
		else
		{
			memcpy(bmp->Palette, data + dataInd, BMP_PALETTE_SIZE);
			dataInd += BMP_PALETTE_SIZE;
		}
	}
	else /* Not an indexed image */
	{
		bmp->Palette = NULL;
	}

	pck_dst = gf_filter_pck_new_alloc(ctx->opid, bmp->Header.Width * bmp->Header.Height * bpp, &bmp->Data);
	if (!pck_dst)
		return GF_OUT_OF_MEM;

	/* Allocate memory for image data */
	/*bmp->Data = (UCHAR*) malloc( bmp->Header.ImageDataSize );*/
	// bmp->Data = (UCHAR*) malloc( bmp->Header.Width* bmp->Header.Height * 4); /* forcing RGBA output*/
	if (bmp->Data == NULL)
	{
		free(bmp->Palette);
		free(bmp);
		return GF_OUT_OF_MEM;
	}

	/* Read image data */
	if (dataInd + bmp->Header.ImageDataSize > size)
	{
		free(bmp->Data);
		free(bmp->Palette);
		free(bmp);
		return GF_CORRUPTED_DATA;
	}
	// TODO:
	//  handle encoded
	//  add in palette. currently 8bpp unmapped
	else
	{
		if (bmp->Header.BitsPerPixel == 32) // nothing much to do for this case; unless in BGR format
		{
			// FIXME : Image is upside down :(
			memcpy( bmp->Data, data+dataInd, bmp->Header.Width* bmp->Header.Height * bpp);
		}
		else if (bmp->Header.BitsPerPixel == 24)
		{ /* we need to insert that alpha; rather than memcpy RGB chunks, let's go one-by-one in case we need to debug  */
			int stride = bmp->Header.Width;
			if (bmp->Header.Width % 4 != 0)
				stride = bmp->Header.Width + (4 - (bmp->Header.Width % 4)); // typically 4-byte aligned
			int diff = bmp->Header.FileSize - (stride * bmp->Header.Height * 3) - dataInd;
			if (diff > 4 || diff < 0)
			{
				printf("error in stride\n");
				return GF_CORRUPTED_DATA;
			} // need to handcheck if not aligned

			UCHAR *tmp = bmp->Data;

			// for (i=0; i<bmp->Header.Height; ++i) // if flipped vertically
			for (i = (bmp->Header.Height) - 1; i > -1; --i)
			{
				for (j = 0; j < bmp->Header.Width * 3; j = j + 3)
				{
					// typically stored in BGR so switch to RGB
					if (i < 987)
					{
						unsigned char *ctmp;
						*ctmp = *(data + dataInd + i * stride * 3 + j);
					}
					*(tmp) = *(data + dataInd + i * stride * 3 + j + 2);
					++tmp;
					*(tmp) = *(data + dataInd + i * stride * 3 + j + 1);
					++tmp;
					*(tmp) = *(data + dataInd + i * stride * 3 + j);
					++tmp;
					*(tmp) = 255;
					++tmp;
				}
			}
		}
		else if (bmp->Header.BitsPerPixel == 8)
		{ /* we need to expand to RGB and insert the alpha  */
			UCHAR *tmp = bmp->Data;
			for (i = 0; i < bmp->Header.Height * bmp->Header.Width; ++i)
			{
				*(tmp) = *(data + dataInd + i);
				++tmp;
				*(tmp) = *(data + dataInd + i);
				++tmp;
				*(tmp) = *(data + dataInd + i);
				++tmp;
				*(tmp) = 255;
				++tmp;
			}
		}
	}
	// pass on back
	// Keeping the width/height etc as funcs rahter than just accessing the structure, in case we move this
	// code elsewhere
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_WIDTH, &PROP_UINT(BMP_GetWidth()));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_HEIGHT, &PROP_UINT(BMP_GetHeight()));

	// Only doing RGBA output at the moment
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_PIXFMT, &PROP_UINT(GF_PIXEL_RGBX));
	gf_filter_pid_set_property(ctx->opid, GF_PROP_PID_STRIDE, &PROP_UINT(bpp * BMP_GetWidth()));

	gf_filter_pck_merge_properties(pck, pck_dst);
	gf_filter_pck_set_dependency_flags(pck_dst, 0);
	gf_filter_pck_send(pck_dst);
	gf_filter_pid_drop_packet(ctx->ipid);

	// TODO: free the local data
	free(bmp);
	
	return GF_OK;
}

/**************************************************************
	Returns the decoded image
**************************************************************/
unsigned int *BMP_GetImage(void)
{
	return ((unsigned int *)bmp->Data);
}

/**************************************************************
	Returns the image's width.
**************************************************************/
int BMP_GetWidth()
{
	if (bmp == NULL)
	{
		return GF_BAD_PARAM;
	}

	return (bmp->Header.Width);
}

/**************************************************************
	Returns the image's height.
**************************************************************/
int BMP_GetHeight()
{
	if (bmp == NULL)
	{
		return GF_BAD_PARAM;
	}

	return (bmp->Header.Height);
}

/**************************************************************
	Returns the image's color depth (bits per pixel).
**************************************************************/
// Useful if want to send info about the image
// int BMP_GetDepth( )
// {
// 	if ( bmp == NULL )
// 	{
// 		return GF_BAD_PARAM;
// 	}
// 	return ( (int) bmp->Header.BitsPerPixel );
// }

/*********************************** Private methods **********************************/

/**************************************************************
	Reads the BMP file's header into the data structure.
	Returns GF_OK on success.
**************************************************************/
GF_Err ReadHeader(unsigned char *bmp_data, const int size)
{
	if (bmp == NULL)
	{
		return GF_BAD_PARAM;
	}

	/* The header's fields are read one by one, and converted from the format's
	little endian to the system's native representation. */
	if (ReadUSHORT(&(bmp->Header.Magic), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.FileSize), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUSHORT(&(bmp->Header.Reserved1), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUSHORT(&(bmp->Header.Reserved2), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.DataOffset), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.HeaderSize), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.Width), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.Height), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUSHORT(&(bmp->Header.Planes), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUSHORT(&(bmp->Header.BitsPerPixel), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.CompressionType), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.ImageDataSize), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.HPixelsPerMeter), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.VPixelsPerMeter), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.ColorsUsed), bmp_data, size) != GF_OK)
		return GF_IO_ERR;
	if (ReadUINT(&(bmp->Header.ColorsRequired), bmp_data, size) != GF_OK)
		return GF_IO_ERR;

	return GF_OK;
}

/**************************************************************
	Reads a little-endian unsigned int from the file.
	Returns non-zero on success.
**************************************************************/
GF_Err ReadUINT(UINT *x, unsigned char *bmp_data, const int size)
{
	UCHAR little[4]; /* BMPs use 32 bit ints */

	if (x == NULL || (dataInd + 4) > size)
	{
		return GF_BAD_PARAM;
	}

	memcpy(&little[0], bmp_data + dataInd, 4);
	dataInd = dataInd + 4;

	*x = (little[3] << 24 | little[2] << 16 | little[1] << 8 | little[0]);

	return GF_OK;
}

/**************************************************************
	Reads a little-endian unsigned short int from the file.
	Returns non-zero on success.
**************************************************************/
GF_Err ReadUSHORT(USHORT *x, unsigned char *bmp_data, const int size)
{
	UCHAR little[2]; /* BMPs use 16 bit shorts */

	if (x == NULL || (dataInd + 2) > size)
	{
		return GF_BAD_PARAM;
	}

	memcpy(&little[0], bmp_data + dataInd, 2);
	dataInd = dataInd + 2;

	*x = (little[1] << 8 | little[0]);

	return GF_OK;
}

GF_FilterRegister BMPFullRegister = {
	.name = "BMPFull",
	.version = "1.0.1",
	GF_FS_SET_DESCRIPTION("BMPFull decoder")
		GF_FS_SET_HELP("This filter decodes limited BMP streams.")
			.private_size = sizeof(GF_BMPFullCtx),
	.priority = 1,
	SETCAPS(BMPFullCaps),
	.configure_pid = BMPFull_configure_pid,
	.process = bmpFullDec_process,
};

const GF_FilterRegister *dynCall_BMPFull_register(GF_FilterSession *session)
{
	return &BMPFullRegister;
}
