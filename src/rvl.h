#ifndef RVL_H
#define RVL_H

#include <stdint.h>
#include <stdio.h>

#define RVL_VERSION_MAJOR 1
#define RVL_VERSION_MINOR 0

typedef uint8_t       RVLByte_t;
typedef uint32_t      RVLSize_t;
typedef const uint8_t RVLConstByte_t;

typedef enum {
   RVLDataFormat_Unsigned = 0,
   RVLDataFormat_Signed   = 1,
   RVLDataFormat_IEEE754  = 2,
} RVLDataFormat_t;

typedef enum {
   RVLDataDimen_Scalar = 1,
   RVLDataDimen_Vec2   = 2,
   RVLDataDimen_Vec3   = 3,
   RVLDataDimen_Vec4   = 3,
} RVLDataDimen_t;

typedef enum {
   RVLGridType_Cartesian   = 0,
   RVLGridType_Regular     = 1,
   RVLGridType_Rectilinear = 2,
} RVLGridType_t;

typedef enum {
   RVLBitDepth_8Bits   = 8,
   RVLBitDepth_16Bits  = 16,
   RVLBitDepth_32Bits  = 32,
   RVLBitDepth_64Bits  = 64,
   RVLBitDepth_128Bits = 128,
} RVLBitDepth_t;

typedef enum {
   RVLEndian_Big    = 0,
   RVLEndian_Little = 1,
} RVLEndian_t;

// -128 - N/A or unknown, n - 1.0e+n meter
typedef enum {
   RVLGridUnit_NA         = -128,
   RVLGridUnit_Meter      = 0,
   RVLGridUnit_Kilometer  = 3,
   RVLGridUnit_Centimeter = -2,
   RVLGridUnit_Milimeter  = -3,
} RVLGridUnit_t;


typedef struct {
   FILE *io;
} RVL_t;

#define SHIFT32(byte, numBits) ((uint32_t)(byte) << numBits)

// Little-endian
#define CHUNK_CODE(b1, b2, b3, b4)                                             \
   SHIFT32(b4, 24) | SHIFT32(b3, 16) | SHIFT32(b2, 8) | b1

typedef enum {
   RVL_CHUNK_CODE_INFO = CHUNK_CODE(73, 78, 70, 79),
   RVL_CHUNK_CODE_DATA = CHUNK_CODE(68, 65, 84, 65),
   RVL_CHUNK_CODE_TEXT = CHUNK_CODE(84, 69, 88, 84),
   RVL_CHUNK_CODE_END  = CHUNK_CODE(69, 78, 68, 32),
} RVLChunkCode_t;

// vHDR
// 1 * 2 + 1 * 6 + 4 * 3 + 4 * 3 + 4 * 3 = 44 bytes
// Padding with 32-bit boundary
typedef struct {
   RVLByte_t       version[2];  // major, minor
   RVLGridType_t   gridType;
   RVLGridUnit_t   gridUnit;
   RVLDataFormat_t dataFormat;
   RVLDataDimen_t  dataDimen;
   RVLBitDepth_t   bitDepth;
   RVLEndian_t     endian;
   //  -----------------   8 bytes
   uint32_t resolution[3];
   float    voxelSize[3];
   float    originCoord[3];
} RVLInfo_t;

typedef struct {
   char *key;
   char *value;
} RVLText_t;

void rvl_write_file_sig(RVL_t *self);
void rvl_write_INFO(RVL_t *self, const RVLInfo_t *info);
void rvl_write_DATA(RVL_t *self, RVLConstByte_t *data, RVLSize_t size);
void rvl_write_TEXT(RVL_t *self, const RVLText_t *arrText, int numText);
void rvl_write_END(RVL_t *self);

#endif

