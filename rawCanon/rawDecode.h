

#ifndef RAW_ACC_H
#define RAW_ACC_H

#ifdef __cplusplus
extern "C" {
#endif

//general error value, used in verification process
#define BEV_OK 0 // this is returned if all is going well
#define BEV_NOT_OK 1 // this is returned on a complete failure; there should be intervening failure reports

// these are returned in component-level messages
#define BEV_COMPONENT_OK 0
#define BEV_NOT_SUPPORTED 0x01
#define BEV_COMPONENT_NOT_SUPPORTED 0x02 // component not supported
#define BEV_SUBFORMAT_NOT_SUPPORTED 0x04 // portion of component not supported
#define BEV_COMPONENT_NOT_FULLY_SUPPORTED 0x08; // something has gone wrong, e.g. too many tags, unknown tags,...




//#define ushort unsigned short 


//global params
#define MAX_NUM_THUMBNAILS 5
#define MAX_NUM_IFDS 10
#define MAX_NUM_COMPONENTS 20  // components struct includes IFDS (main, thumbs, XMPs, metadata)
#define MAX_METADATA_LENGTH 100000
#define STRLIM 500 // this is failsafe for non-null terminated strings; should not be reached

// These enums do not correspond to the DNG compression types
// e.g., baseline and lossless JPEG share comp flag = 7 in
// DNG. These are returned as a flag from the DNG parser for
// the demuxer to choose the accessor.

enum DNG_COMPRESSION_TYPE {
COMP_UNKNOWN   = 0,            /* placeholder when we don't know type */
NO_COMPRESSION = 1,           /* no compression */
UNCOMPRESSED   = 2,		/* uncompressed */
PACKED_DNG     = 3,           /* packed DNG, uncompressed or Huffman */
LOSSLESS_JPEG  = 4,		/* lossless JPEG */
BASELINE_JPEG  = 5,		/* baseline JPEG or lossless JPEG */
ZIP            = 6,           /* ZIP */
LOSSY_JPEG     = 7		/* Lossy JPEG */
};

char* compressionTypeToName(int type)
{
    switch (type)
    {
    case 1: return "None"; break;
    case 2: return "Uncompressed"; break;
    case 3: return "Packed DNG"; break;
    case 4: return "JPEG - lossless"; break;
    case 5: return "JPEG - baseline"; break;
    case 6: return "ZIP"; break;
    default: return "UNKNOWN"; break;
    }
}


enum DNG_IFD_TYPE {
    TYPE_UNKNOWN = 0,
    TYPE_MAIN_IMAGE = 1,
    TYPE_THUMBNAIL = 2,
    TYPE_XMP = 3,
	TYPE_METADATA = 4
};

char* IFDtypeToName(int type)
{
    switch (type)
    {
    case 1: return "MAIN_IMAGE"; break;
    case 2: return "THUMBNAIL"; break;
    case 3: return "XMP"; break;
    case 4: return "METADATA"; break;
    default: return "UNKNOWN"; break;
    }
}

  enum DNG_UNICODE_TYPE {  /* used for XMP */
    UNICODE_UNKNOWN =0,
    UNICODE8 = 1,
    UNICODE16 = 2,
    UNICODE32 =3
  };

typedef struct tiff_ifd_ {
  int type;
  int raw_comp; /* note different index from comp below */
 
  long XMP_start; /* handle XMP data within an ifd */
  long XMP_stop;
  int  XMP_coding;

  int width, height, bps[3], comp, phint, offset, flip, samples, bytes, endoffset;
  
  int tile_width, tile_length;
  short planarConf;
  long rowsPerStrip;
  float shutter;
  int message; //this indicates content, decodability, etc. keep as int, may later extend return more detailed messages
} TIFF_IFD;

extern TIFF_IFD tiff_ifd[MAX_NUM_IFDS];


// we need to track each component, since multiple can be embedded in an
// IFD; separate out and link back to host IFD
typedef struct component_ {
  int type;
  int ifd_loc; 
  int width;
  int height;
  int size;
  int message; //this indicates content, decodability, etc. keep as int, may later extend return more detailed messages
} COMPONENT;

extern COMPONENT component[MAX_NUM_COMPONENTS];

extern int outwidth, outheight;
extern unsigned char* outbuf;

extern int imgSetup();

extern int getNumComponents(char *inbuf, int insize );

extern int getWidth(int idx);
extern int getHeight(int idx);
extern int getSize(int idx);
extern int getComponentType(int idx);
extern unsigned char*  getData(int idx);
extern int getMeta(int idx, unsigned char* metabuf); // used to return data about the component
extern int imgShutdown();

#ifdef __cplusplus
}
#endif


#endif
