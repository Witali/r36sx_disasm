#ifndef R36SX_WINDOWS_FREETYPE_H
#define R36SX_WINDOWS_FREETYPE_H

#include <stdint.h>

typedef int FT_Error;
typedef long FT_Long;
typedef unsigned int FT_UInt;
typedef unsigned long FT_ULong;
typedef int32_t FT_Int32;
typedef int FT_Encoding;
typedef void *FT_Library;

typedef struct FT_Vector_ {
    long x;
    long y;
} FT_Vector;

typedef struct FT_Bitmap_ {
    unsigned int rows;
    unsigned int width;
    int pitch;
    unsigned char *buffer;
    unsigned short num_grays;
    unsigned char pixel_mode;
    unsigned char palette_mode;
    void *palette;
} FT_Bitmap;

typedef struct FT_GlyphSlotRec_ {
    FT_Bitmap bitmap;
    int bitmap_left;
    int bitmap_top;
    FT_Vector advance;
} FT_GlyphSlotRec, *FT_GlyphSlot;

typedef struct FT_FaceRec_ {
    FT_GlyphSlot glyph;
} FT_FaceRec, *FT_Face;

#define FT_ENCODING_UNICODE 0x756e6963
#define FT_LOAD_RENDER 0x00000004
#define FT_LOAD_TARGET_LIGHT 0x00010000
#define FT_PIXEL_MODE_GRAY 2

#endif
