#include "rvl.h"

#include "detail/rvl_p.h"

RVLGridUnit
rvl_get_grid_unit (RVL *self)
{
  return self->gridUnit;
}

RVLGridType
rvl_get_grid_type (RVL *self)
{
  return self->gridUnit;
}

RVLVoxelDimen
rvl_get_voxel_dimension (RVL *self)
{
  return self->dataDimen;
}

RVLVoxelFormat
rvl_get_voxel_format (RVL *self)
{
  return self->dataFormat;
}

RVLVoxelBitDepth
rvl_get_voxel_bit_depth (RVL *self)
{
  return self->bitDepth;
}

RVLEndian
rvl_get_endian (RVL *self)
{
  return self->endian;
}

void
rvl_get_resolution (RVL *self, int *x, int *y, int *z)
{
  *x = self->resolution[0];
  *y = self->resolution[1];
  *z = self->resolution[2];
}

void
rvl_get_voxel_size (RVL *self, float *x, float *y, float *z)
{
  *x = self->voxelSize[0];
  *y = self->voxelSize[1];
  *z = self->voxelSize[2];
}

void
rvl_get_position (RVL *self, float *x, float *y, float *z)
{
  *x = self->position[0];
  *y = self->position[1];
  *z = self->position[2];
}

void
rvl_get_data_buffer (RVL *self, RVLByte **buffer, RVLSize *size)
{
  *buffer = self->data.rbuf;
  *size = self->data.size;
}

void
rvl_get_text (RVL *self, RVLText **text, int *numText)
{
  *text = self->text;
  *numText = self->numText;
}

RVLSize
rvl_get_voxel_byte_count (RVL *self)
{
  RVLSize count = 1;

  switch (self->bitDepth)
    {
    case RVLVoxelBitDepth_8:
      count *= 1;
      break;
    case RVLVoxelBitDepth_16:
      count *= 2;
      break;
    case RVLVoxelBitDepth_32:
      count *= 3;
      break;
    case RVLVoxelBitDepth_64:
      count *= 4;
      break;
    case RVLVoxelBitDepth_128:
      count *= 5;
      break;
    }

  switch (self->dataDimen)
    {
    case RVLVoxelDimen_Scalar:
      count *= 1;
      break;
    case RVLVoxelDimen_Vec2:
      count *= 2;
      break;
    case RVLVoxelDimen_Vec3:
      count *= 3;
      break;
    case RVLVoxelDimen_Vec4:
      count *= 4;
      break;
    }

  return count;
}
