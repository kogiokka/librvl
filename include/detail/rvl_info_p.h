#ifndef RVL_INFO_P_H
#define RVL_INFO_P_H

#include "rvl_p.h"

struct RVLInfo
{
  RVLGridType_t gridType;
  RVLGridUnit_t gridUnit;
  RVLDataFormat_t dataFormat;
  RVLBitDepth_t bitDepth;
  RVLDataDimen_t dataDimen;
  RVLEndian_t endian;
  u32 resolution[3];
  f32 voxelSize[3];
  f32 position[3];
};

#endif
