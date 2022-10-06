#ifndef RVL_DEFS_H
#define RVL_DEFS_H

#include <stdint.h>
#include <stdio.h>

#define RVL_VERSION_MAJOR 0
#define RVL_VERSION_MINOR 1

typedef float f32;
typedef uint8_t u8;
typedef uint32_t u32;
typedef const uint8_t u8const;

#define SHIFT32(byte, numBits) ((u32)(byte) << numBits)

// Little-endian
#define CHUNK_CODE(b1, b2, b3, b4)                                            \
  SHIFT32 (b4, 24) | SHIFT32 (b3, 16) | SHIFT32 (b2, 8) | b1

typedef enum
{
  RVLDataFormat_Unsigned = 0,
  RVLDataFormat_Signed = 1,
  RVLDataFormat_IEEE754 = 2,
} RVLDataFormat_t;

typedef enum
{
  RVLDataDimen_Scalar = 1,
  RVLDataDimen_Vec2 = 2,
  RVLDataDimen_Vec3 = 3,
  RVLDataDimen_Vec4 = 3,
} RVLDataDimen_t;

typedef enum
{
  RVLGridType_Cartesian = 0,
  RVLGridType_Regular = 1,
  RVLGridType_Rectilinear = 2,
} RVLGridType_t;

typedef enum
{
  RVLBitDepth_8Bits = 8,
  RVLBitDepth_16Bits = 16,
  RVLBitDepth_32Bits = 32,
  RVLBitDepth_64Bits = 64,
  RVLBitDepth_128Bits = 128,
} RVLBitDepth_t;

typedef enum
{
  RVLEndian_Big = 0,
  RVLEndian_Little = 1,
} RVLEndian_t;

// -128 - N/A or unknown, n - 1.0e+n meter
typedef enum
{
  RVLGridUnit_NA = -128,
  RVLGridUnit_Meter = 0,
  RVLGridUnit_Kilometer = 3,
  RVLGridUnit_Centimeter = -2,
  RVLGridUnit_Milimeter = -3,
} RVLGridUnit_t;

typedef enum
{
  RVLIoState_Read,
  RVLIoState_Write,
} RVLIoState_t;

typedef enum
{
  RVLChunkCode_INFO = CHUNK_CODE (73, 78, 70, 79),
  RVLChunkCode_DATA = CHUNK_CODE (68, 65, 84, 65),
  RVLChunkCode_TEXT = CHUNK_CODE (84, 69, 88, 84),
  RVLChunkCode_END = CHUNK_CODE (69, 78, 68, 32),
} RVLChunkCode_t;

struct RVLText;
struct RVLInfo;
struct RVLData;

typedef struct
{
  FILE *io;
  u8 version[2]; // major, minor
  RVLIoState_t ioState;

  /* INFO chunk */
  struct RVLInfo *info;

  /* TEXT chunk */
  struct RVLText *text;
  int numText;

  /* DATA chunk */
  struct RVLData* data;
} RVL_t;

#endif
