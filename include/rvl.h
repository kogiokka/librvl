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
 * Currently, there are 5 types of chunk: VHDR, GRID, DATA, TEXT, VEND.
 * A valid RVL file must contain an VHDR chunk, an GRID chunk, one or more DATA
 * chunks, and an VEND chunk.
 *
 * VHDR Chunk
 * ++++++++++
 *
 * VHDR chunk holds the information on how to interpret the DATA chunk. The
 * layout of the chunk payload is as follow:
 *
 * +00 1B    librvl major version
 * +01 1B    librvl minor version
 * +02 12B   resolution
 * +14 2B    primitive
 * +16 1B    endian
 * +17 1B    padding byte
 *
 * Total 18 bytes
 *
 *
 * GRID Chunk
 * ++++++++++
 *
 * +00   1B   grid type
 * +01   1B   grid unit
 * +02  12B   position of the lower corner of the grid
 * +14   nB   voxel dimensions
 *
 *
 * Each grid type has its own voxel dimensions configuration.
 *
 * Regular Grid
 * ------------
 *
 * +14  12B   three values for x, y, and z, respectively.
 *
 * Rectilinear Grid
 * ----------------
 *
 * +14 [voxel dimensions in x-direction]
 * +xx [voxel dimensions in y-direction]
 * +xx [voxel dimensions in z-direction]
 *
 */

#ifndef RVL_H
#define RVL_H

#include <stdint.h>

#define RVL_VERSION_MAJOR 0
#define RVL_VERSION_MINOR 4

/* RVL struct types */
typedef struct RVL     RVL;
typedef struct RVLText RVLText;

/* enum types */
typedef unsigned int RVLenum;

/* enum definitions */
#define RVL_PRIMITIVE_U8   0x0103
#define RVL_PRIMITIVE_U16  0x0104
#define RVL_PRIMITIVE_U32  0x0105
#define RVL_PRIMITIVE_U64  0x0106
#define RVL_PRIMITIVE_U128 0x0107
#define RVL_PRIMITIVE_I8   0x0113
#define RVL_PRIMITIVE_I16  0x0114
#define RVL_PRIMITIVE_I32  0x0115
#define RVL_PRIMITIVE_I64  0x0116
#define RVL_PRIMITIVE_I128 0x0117
#define RVL_PRIMITIVE_F8   0x0123
#define RVL_PRIMITIVE_F16  0x0124
#define RVL_PRIMITIVE_F32  0x0125
#define RVL_PRIMITIVE_F64  0x0126
#define RVL_PRIMITIVE_F128 0x0127

#define RVL_PRIMITIVE_VEC2U8   0x0203
#define RVL_PRIMITIVE_VEC2U16  0x0204
#define RVL_PRIMITIVE_VEC2U32  0x0205
#define RVL_PRIMITIVE_VEC2U64  0x0206
#define RVL_PRIMITIVE_VEC2U128 0x0207
#define RVL_PRIMITIVE_VEC2I8   0x0213
#define RVL_PRIMITIVE_VEC2I16  0x0214
#define RVL_PRIMITIVE_VEC2I32  0x0215
#define RVL_PRIMITIVE_VEC2I64  0x0216
#define RVL_PRIMITIVE_VEC2I128 0x0217
#define RVL_PRIMITIVE_VEC2F8   0x0223
#define RVL_PRIMITIVE_VEC2F16  0x0224
#define RVL_PRIMITIVE_VEC2F32  0x0225
#define RVL_PRIMITIVE_VEC2F64  0x0226
#define RVL_PRIMITIVE_VEC2F128 0x0227

#define RVL_PRIMITIVE_VEC3U8   0x0303
#define RVL_PRIMITIVE_VEC3U16  0x0304
#define RVL_PRIMITIVE_VEC3U32  0x0305
#define RVL_PRIMITIVE_VEC3U64  0x0306
#define RVL_PRIMITIVE_VEC3U128 0x0307
#define RVL_PRIMITIVE_VEC3I8   0x0313
#define RVL_PRIMITIVE_VEC3I16  0x0314
#define RVL_PRIMITIVE_VEC3I32  0x0315
#define RVL_PRIMITIVE_VEC3I64  0x0316
#define RVL_PRIMITIVE_VEC3I128 0x0317
#define RVL_PRIMITIVE_VEC3F8   0x0323
#define RVL_PRIMITIVE_VEC3F16  0x0324
#define RVL_PRIMITIVE_VEC3F32  0x0325
#define RVL_PRIMITIVE_VEC3F64  0x0326
#define RVL_PRIMITIVE_VEC3F128 0x0327

#define RVL_PRIMITIVE_VEC4U8   0X0403
#define RVL_PRIMITIVE_VEC4U16  0X0404
#define RVL_PRIMITIVE_VEC4U32  0X0405
#define RVL_PRIMITIVE_VEC4U64  0X0406
#define RVL_PRIMITIVE_VEC4U128 0X0407
#define RVL_PRIMITIVE_VEC4I8   0X0413
#define RVL_PRIMITIVE_VEC4I16  0X0414
#define RVL_PRIMITIVE_VEC4I32  0X0415
#define RVL_PRIMITIVE_VEC4I64  0X0416
#define RVL_PRIMITIVE_VEC4I128 0X0417
#define RVL_PRIMITIVE_VEC4F8   0X0423
#define RVL_PRIMITIVE_VEC4F16  0X0424
#define RVL_PRIMITIVE_VEC4F32  0X0425
#define RVL_PRIMITIVE_VEC4F64  0X0426
#define RVL_PRIMITIVE_VEC4F128 0X0427

#define RVL_ENDIAN_BIG    0
#define RVL_ENDIAN_LITTLE 1

#define RVL_GRID_REGULAR     0
#define RVL_GRID_RECTILINEAR 1

#define RVL_UNIT_NA        0X00
#define RVL_UNIT_METER     0X01
#define RVL_UNIT_MILIMETER 0X02
#define RVL_UNIT_KILOMETER 0X03

#define RVL_COMPRESSION_LZMA 0x00
#define RVL_COMPRESSION_LZ4  0x01

// Prevent name-mangling for librvl functions from C++ compiler.
#ifdef __cplusplus
extern "C"
{
#endif

  RVL *rvl_create_writer (const char *filename);
  RVL *rvl_create_reader (const char *filename);
  void rvl_destroy (RVL **self);

  // Write the entire rvl file
  void rvl_write_rvl (RVL *self);

  // Read the entire rvl file
  void rvl_read_rvl (RVL *self);

  // Read the information without the actual volumetric data
  // Use rvl_get_* to retrieve information.
  void rvl_read_info (RVL *self);

  // Read the entire volumetric data into the buffer. The buffer allocation and
  // deallocation is managed by the user.
  void rvl_read_data_buffer (RVL *self, unsigned char **buffer);

  /* VHDR chunk functions */
  void    rvl_set_resolution (RVL *self, int x, int y, int z);
  void    rvl_set_primitive (RVL *self, RVLenum primitive);
  void    rvl_set_endian (RVL *self, RVLenum endian);
  void    rvl_set_compression (RVL *self, RVLenum compression);
  void    rvl_get_resolution (RVL *self, int *x, int *y, int *z);
  RVLenum rvl_get_primitive (RVL *self);
  RVLenum rvl_get_endian (RVL *self);
  RVLenum rvl_get_compression (RVL *self);

  /* GRID chunk functions */
  void    rvl_set_grid_type (RVL *self, RVLenum gridType);
  void    rvl_set_grid_unit (RVL *self, RVLenum gridUnit);
  void    rvl_set_grid_position (RVL *self, float x, float y, float z);
  RVLenum rvl_get_grid_type (RVL *self);
  RVLenum rvl_get_grid_unit (RVL *self);
  void    rvl_get_grid_position (RVL *self, float *x, float *y, float *z);

  void rvl_set_voxel_dims (RVL *self, float dx, float dy, float dz);
  void rvl_get_voxel_dims (RVL *self, float *dx, float *dy, float *dz);

  void rvl_set_voxel_dims_v (RVL *self, int ndx, int ndy, int ndz, float *dx,
                             float *dy, float *dz);
  void rvl_get_voxel_dims_v (RVL *self, int *ndx, int *ndy, int *ndz,
                             const float **dx, const float **dy,
                             const float **dz);

  /* DATA chunk functions */

  // Set the data buffer to be written by the RVL writer. The RVL instance does
  // not own the pointer; the user should allocate the memory before writing
  // and deallocate the memory after writing.
  void rvl_set_data_buffer (RVL *self, unsigned int size,
                            const unsigned char *buffer);

  // Get the data buffer from the RVL reader. The RVL instance owns the
  // pointer, and users should not free the memory themselves.
  void rvl_get_data_buffer (RVL *self, unsigned char **buffer);

  /* TEXT chunk functions */
  RVLText *rvl_text_create_array (int num);
  void     rvl_text_destroy_array (RVLText **self);
  void     rvl_set_text (RVL *self, RVLText **text, int numText);
  void     rvl_get_text (RVL *self, RVLText **text, int *numText);
  void     rvl_text_set (RVLText *textArr, int index, char *key, char *value);
  void     rvl_text_get (RVLText *textArr, int index, const char **key,
                         const char **value);

  /**
   * Helpers
   *
   * These helper functions depend on certain RVL information. Please make sure
   * the instance has been fully configured before using them.
   */
  unsigned int rvl_get_primitive_nbytes (RVL *self);
  unsigned int rvl_get_data_nbytes (RVL *self);

#ifdef __cplusplus
}
#endif

#endif
