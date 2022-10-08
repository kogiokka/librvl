#ifndef RVL_P_H
#define RVL_P_H

#include <stdint.h>
#include <stdio.h>

#include "../rvl.h"

typedef float f32;
typedef uint8_t u8;
typedef uint32_t u32;
typedef const uint8_t rvlcbyte_t;

/**
 * Chunk code constants
 *
 * A chunk code is an unsigned integer that consists of human readable
 * characters. CHUNK_CODE macro puts the code characters into the integer in
 * little-endian order.
 */
#define SHIFT32(byte, numBits) ((u32)(byte) << numBits)
#define CHUNK_CODE(b1, b2, b3, b4)                                            \
  (SHIFT32 (b4, 24) | SHIFT32 (b3, 16) | SHIFT32 (b2, 8) | b1)

typedef uint32_t RVLChunkCode_t;
#define RVLChunkCode_INFO CHUNK_CODE (73, 78, 70, 79)
#define RVLChunkCode_DATA CHUNK_CODE (68, 65, 84, 65)
#define RVLChunkCode_TEXT CHUNK_CODE (84, 69, 88, 84)
#define RVLChunkCode_END CHUNK_CODE (69, 78, 68, 32)

// RVL File Signature: .RVL FORMAT\0
#define RVL_FILE_SIG_SIZE 12
extern rvlbyte_t RVL_FILE_SIG[RVL_FILE_SIG_SIZE];

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
