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
 * +05 1B    bit depth
 * +06 1B    data dimensions
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

#include <stdint.h>

#define RVL_VERSION_MAJOR 0
#define RVL_VERSION_MINOR 1

typedef struct RVL RVL;
typedef struct RVLText RVLText;

typedef uint8_t RVLByte;
typedef uint32_t RVLSize;

typedef uint8_t RVLDataFormat;
typedef uint8_t RVLBitDepth;
typedef uint8_t RVLDataDimen;

typedef uint8_t RVLGridType;
typedef uint8_t RVLEndian;
typedef int8_t RVLGridUnit;

#define RVLDataFormat_Unsigned 0
#define RVLDataFormat_Signed 1
#define RVLDataFormat_IEEE754 2

#define RVLDataDimen_Scalar 1
#define RVLDataDimen_Vec2 2
#define RVLDataDimen_Vec3 3
#define RVLDataDimen_Vec4 3

#define RVLGridType_Cartesian 0
#define RVLGridType_Regular 1
#define RVLGridType_Rectilinear 2

#define RVLBitDepth_8Bits 8
#define RVLBitDepth_16Bits 16
#define RVLBitDepth_32Bits 32
#define RVLBitDepth_64Bits 64
#define RVLBitDepth_128Bits 128

#define RVLEndian_Big 0
#define RVLEndian_Little 1

// -128 - N/A or unknown, n - 1.0e+n meter
#define RVLGridUnit_NA -128
#define RVLGridUnit_Milimeter -3
#define RVLGridUnit_Centimeter -2
#define RVLGridUnit_Meter 0
#define RVLGridUnit_Kilometer 3

RVL *rvl_create_writer (const char *filename);
RVL *rvl_create_reader (const char *filename);
void rvl_destroy (RVL **self);

void rvl_write (RVL *self);
void rvl_read (RVL *self);

void rvl_set_grid_type (RVL *self, RVLGridType gridType);
void rvl_set_grid_unit (RVL *self, RVLGridUnit gridUnit);
void rvl_set_data_format (RVL *self, RVLDataFormat format);
void rvl_set_data_dimensions (RVL *self, RVLDataDimen dimen);
void rvl_set_data_bit_depth (RVL *self, RVLBitDepth bitDepth);
void rvl_set_endian (RVL *self, RVLEndian endian);

void rvl_set_resolution (RVL *self, int x, int y, int z);
void rvl_set_voxel_size (RVL *self, float x, float y, float z);
void rvl_set_position (RVL *self, float x, float y, float z);

RVLGridType rvl_get_grid_type (RVL *self);
RVLGridUnit rvl_get_grid_unit (RVL *self);
RVLDataDimen rvl_get_data_dimensions (RVL *self);
RVLDataFormat rvl_get_data_format (RVL *self);
RVLBitDepth rvl_get_data_bit_depth (RVL *self);
RVLEndian rvl_get_endian (RVL *self);

void rvl_get_resolution (RVL *self, int *x, int *y, int *z);
void rvl_get_voxel_size (RVL *self, float *x, float *y, float *z);
void rvl_get_position (RVL *self, float *x, float *y, float *z);

RVLText *rvl_text_create_array (int num);
void rvl_text_destroy_array (RVLText **self);
void rvl_set_text (RVL *self, RVLText **text, int numText);
void rvl_get_text (RVL *self, RVLText **text, int *numText);
void rvl_text_set (RVLText *textArr, int index, char *key, char *value);
void rvl_text_get (RVLText *textArr, int index, const char **key,
                   const char **value);

void rvl_alloc_data_buffer (RVL *self);
void rvl_set_sub_data_buffer (RVL *self, RVLSize offset, RVLSize size,
                              RVLByte *data);
void rvl_get_data_buffer (RVL *self, RVLByte **buffer, RVLSize *size);

#endif
