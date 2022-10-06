#ifndef RVL_INFO_P_H
#define RVL_INFO_P_H

#include "../rvl.h"

struct RVLInfo
{
  RVLInfoGrid_t grid;
  RVLInfoDataForm_t dataForm;
  RVLEndian_t endian;
  RVLVec3u_t resolution;
  RVLVec3f_t voxelSize;
  RVLVec3f_t position;
};

#endif
