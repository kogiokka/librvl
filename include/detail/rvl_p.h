#ifndef RVL_P_H
#define RVL_P_H

#include <stdint.h>
#include <stdio.h>

#include "../rvl.h"

typedef float f32;
typedef uint8_t u8;
typedef uint32_t u32;
typedef const uint8_t rvlcbyte_t;

typedef uint32_t RVLChunkCode_t;

#define RVL_FILE_SIG_SIZE 12

// .RVL FORMAT\0
extern rvlbyte_t RVL_FILE_SIG[RVL_FILE_SIG_SIZE];

#define SHIFT32(byte, numBits) ((u32)(byte) << numBits)

// Little-endian
#define CHUNK_CODE(b1, b2, b3, b4)                                            \
  (SHIFT32 (b4, 24) | SHIFT32 (b3, 16) | SHIFT32 (b2, 8) | b1)

#define RVLChunkCode_INFO CHUNK_CODE (73, 78, 70, 79)
#define RVLChunkCode_DATA CHUNK_CODE (68, 65, 84, 65)
#define RVLChunkCode_TEXT CHUNK_CODE (84, 69, 88, 84)
#define RVLChunkCode_END CHUNK_CODE (69, 78, 68, 32)

struct RVL
{
  FILE *io;
  u8 version[2]; // major, minor
  RVLIoState_t ioState;

  /* INFO chunk */
  RVLInfo_t *info;

  /* TEXT chunk */
  RVLText_t *text;
  int numText;

  /* DATA chunk */
  RVLData_t *data;
};

#endif
