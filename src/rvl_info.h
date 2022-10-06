#ifndef RVL_INFO_H
#define RVL_INFO_H

#include "rvl_defs.h"

typedef struct RVLInfo
{
  RVLGridType_t gridType;
  RVLGridUnit_t gridUnit;
  RVLDataFormat_t dataFormat;
  RVLDataDimen_t dataDimen;
  RVLBitDepth_t bitDepth;
  RVLEndian_t endian;
  uint32_t resolution[3];
  float voxelSize[3];
  float coordinates[3];
} RVLInfo_t;

RVLInfo_t *rvl_info_create ();
void rvl_info_destroy (RVLInfo_t **self);
void rvl_info_set_grid (RVLInfo_t *self, RVLGridType_t gridType,
                        RVLGridUnit_t gridUnit);
void rvl_info_set_data_form (RVLInfo_t *self, RVLDataFormat_t format,
                             RVLBitDepth_t bitDepth, RVLDataDimen_t dimen);
void rvl_info_set_endian (RVLInfo_t *self, RVLEndian_t endian);
void rvl_info_set_resolution (RVLInfo_t *self, u32 x, u32 y, u32 z);
void rvl_info_set_voxel_size (RVLInfo_t *self, f32 x, f32 y, f32 z);

#endif
