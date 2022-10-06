/**
 * rvl.h - Structs and functions for read/write an rvl file.
 *
 * Chunk
 * =====
 *
 * Each chunk consists of three parts: size, code and payload.
 *
 * +00 4B chunk size (N bytes)
 * +04 4B chunk code
 * +08 NB chunk payload
 *
 * Chunk Codes
 * ===========
 *
 * Currently, there are 4 types of chunk: INFO, DATA, TEXT, END.
 *
 * INFO Chunk
 * ++++++++++
 *
 * INFO chunk holds the information on how to interpret the DATA chunk. The
 * layout of the chunk payload is as follow:
 *
 * +00 1B    librvl major version
 * +01 1B    librvl minor version
 * +02 1B    grid type
 * +03 1B    grid unit
 * +04 1B    data format
 * +05 1B    data dimensions
 * +06 1B    bit depth
 * +07 1B    endianness
 * +08 12B   resolution
 * +20 12B   voxel size
 * +32 12B   coordinates of the lower corner of the dataset
 *
 * Total 44 bytes
 *
 */

#ifndef RVL_H
#define RVL_H

#include <stdio.h>

#include "rvl_defs.h"

#include "rvl_data.h"
#include "rvl_info.h"
#include "rvl_text.h"

RVL_t *rvl_create (const char *filename, RVLIoState_t ioState);
void rvl_destroy (RVL_t **selfPtr);

void rvl_set_INFO (RVL_t *self, RVLInfo_t **info);
void rvl_set_TEXT (RVL_t *self, RVLText_t **text, int numText);
void rvl_set_DATA (RVL_t *self, RVLData_t **data);

void rvl_write (RVL_t *self);
void rvl_read (RVL_t *self);

#endif
