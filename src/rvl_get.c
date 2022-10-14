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
  return self->gridType;
}

RVLPrimitive
rvl_get_primitive (RVL *self)
{
  return self->primitive;
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
  *size   = self->data.size;
}

void
rvl_get_text (RVL *self, RVLText **text, int *numText)
{
  *text    = self->text;
  *numText = self->numText;
}

RVLSize
rvl_get_primitive_byte_count (RVL *self)
{
  RVLByte *p = (RVLByte *)&self->primitive;

  u8 dimen = p[1];
  u8 bytes = (1 << (p[0] & 0x0f)) / 8;

  return dimen * bytes;
}
