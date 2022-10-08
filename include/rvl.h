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
typedef struct RVLInfo RVLInfo;
typedef struct RVLData RVLData;
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

void rvl_set_INFO (RVL *self, RVLInfo **info);
void rvl_set_TEXT (RVL *self, RVLText **text, int numText);
void rvl_set_DATA (RVL *self, RVLData **data);

void rvl_get_INFO (RVL *self, RVLInfo **info);
void rvl_get_TEXT (RVL *self, RVLText **text, int *numText);
void rvl_get_DATA (RVL *self, RVLData **data);

void rvl_write (RVL *self);
void rvl_read (RVL *self);

RVLInfo *rvl_info_create ();
void rvl_info_destroy (RVLInfo **self);
void rvl_info_set_grid (RVLInfo *self, RVLGridType gridType,
                        RVLGridUnit gridUnit);
void rvl_info_set_data_form (RVLInfo *self, RVLDataFormat format,
                             RVLBitDepth bits, RVLDataDimen dimen);
void rvl_info_set_endian (RVLInfo *self, RVLEndian endian);
void rvl_info_set_resolution (RVLInfo *self, int x, int y, int z);
void rvl_info_set_voxel_size (RVLInfo *self, float x, float y, float z);
void rvl_info_set_position (RVLInfo *self, float x, float y, float z);

void rvl_info_get_grid (RVLInfo *self, RVLGridType *type, RVLGridUnit *unit);
void rvl_info_get_data_form (RVLInfo *self, RVLDataFormat *format,
                             RVLBitDepth *bits, RVLDataDimen *dimen);
void rvl_info_get_endian (RVLInfo *self, RVLEndian *endian);
void rvl_info_get_resolution (RVLInfo *self, int *x, int *y, int *z);
void rvl_info_get_voxel_size (RVLInfo *self, float *x, float *y, float *z);
void rvl_info_get_position (RVLInfo *self, float *x, float *y, float *z);

RVLText *rvl_text_array_create (int num);
void rvl_text_array_destroy (RVLText **self);
void rvl_text_set (RVLText *textArr, int index, char *key, char *value);
void rvl_text_get (RVLText *textArr, int index, const char **key,
                   const char **value);

RVLData *rvl_data_create ();
void rvl_data_destroy (RVLData **self);
void rvl_data_alloc (RVLData *self, RVLInfo *info);
void rvl_data_set_sub_range (RVLData *self, RVLSize offset, RVLSize size,
                             RVLByte *data);
RVLSize rvl_data_get_buffer (RVLData *self, RVLByte **buffer);

#endif
