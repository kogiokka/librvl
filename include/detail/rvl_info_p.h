#ifndef RVL_INFO_P_H
#define RVL_INFO_P_H

#include "rvl_p.h"

struct RVLInfo
{
  RVLGridType gridType;
  RVLGridUnit gridUnit;
  RVLDataFormat dataFormat;
  RVLBitDepth bitDepth;
  RVLDataDimen dataDimen;
  RVLEndian endian;
  u32 resolution[3];
  f32 voxelSize[3];
  f32 position[3];
};

#endif
