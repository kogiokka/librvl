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

typedef struct RVL RVL_t;
typedef struct RVLInfo RVLInfo_t;
typedef struct RVLData RVLData_t;
typedef struct RVLText RVLText_t;

typedef uint8_t rvlbyte_t;
typedef uint32_t rvlsize_t;

typedef uint8_t RVLDataFormat_t;
typedef uint8_t RVLBitDepth_t;
typedef uint8_t RVLDataDimen_t;

typedef uint8_t RVLGridType_t;
typedef uint8_t RVLEndian_t;
typedef int8_t RVLGridUnit_t;

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

RVL_t *rvl_create_writer (const char *filename);
RVL_t *rvl_create_reader (const char *filename);
void rvl_destroy (RVL_t **self);

void rvl_set_INFO (RVL_t *self, RVLInfo_t **info);
void rvl_set_TEXT (RVL_t *self, RVLText_t **text, int numText);
void rvl_set_DATA (RVL_t *self, RVLData_t **data);

void rvl_get_INFO (RVL_t *self, RVLInfo_t **info);
void rvl_get_TEXT (RVL_t *self, RVLText_t **text, int *numText);
void rvl_get_DATA (RVL_t *self, RVLData_t **data);

void rvl_write (RVL_t *self);
void rvl_read (RVL_t *self);

RVLInfo_t *rvl_info_create ();
void rvl_info_destroy (RVLInfo_t **self);
void rvl_info_set_grid (RVLInfo_t *self, RVLGridType_t gridType,
                        RVLGridUnit_t gridUnit);
void rvl_info_set_data_form (RVLInfo_t *self, RVLDataFormat_t format,
                             RVLBitDepth_t bits, RVLDataDimen_t dimen);
void rvl_info_set_endian (RVLInfo_t *self, RVLEndian_t endian);
void rvl_info_set_resolution (RVLInfo_t *self, int x, int y, int z);
void rvl_info_set_voxel_size (RVLInfo_t *self, float x, float y, float z);
void rvl_info_set_position (RVLInfo_t *self, float x, float y, float z);

void rvl_info_get_grid (RVLInfo_t *self, RVLGridType_t *type,
                        RVLGridUnit_t *unit);
void rvl_info_get_data_form (RVLInfo_t *self, RVLDataFormat_t *format,
                             RVLBitDepth_t *bits, RVLDataDimen_t *dimen);
void rvl_info_get_endian (RVLInfo_t *self, RVLEndian_t *endian);
void rvl_info_get_resolution (RVLInfo_t *self, int *x, int *y, int *z);
void rvl_info_get_voxel_size (RVLInfo_t *self, float *x, float *y, float *z);
void rvl_info_get_position (RVLInfo_t *self, float *x, float *y, float *z);

RVLText_t *rvl_text_array_create (int num);
void rvl_text_array_destroy (RVLText_t **self);
void rvl_text_set (RVLText_t *textArr, int index, char *key, char *value);
void rvl_text_get (RVLText_t *textArr, int index, const char **key,
                   const char **value);

RVLData_t *rvl_data_create ();
void rvl_data_destroy (RVLData_t **self);
void rvl_data_alloc (RVLData_t *self, RVLInfo_t *info);
void rvl_data_set_sub_range (RVLData_t *self, rvlsize_t offset, rvlsize_t size,
                             rvlbyte_t *data);
rvlsize_t rvl_data_get_buffer (RVLData_t *self, rvlbyte_t **buffer);

#endif
