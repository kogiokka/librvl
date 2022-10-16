#include "rvl.h"

#include "detail/rvl_p.h"

RVLGridUnit
rvl_get_grid_unit (RVL *self)
{
  return self->grid.unit;
}

RVLGridType
rvl_get_grid_type (RVL *self)
{
  return self->grid.type;
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
rvl_get_voxel_dimensions (RVL *self, float *x, float *y, float *z)
{
  const float *buf = (const float *)self->grid.vxDimBuf;

  *x = buf[0];
  *y = buf[1];
  *z = buf[2];
}

void
rvl_get_voxel_dimensions_v (RVL *self, const float **dimensions)
{
  *dimensions = (const float *)self->grid.vxDimBuf;
}

void
rvl_get_position (RVL *self, float *x, float *y, float *z)
{
  *x = self->grid.position[0];
  *y = self->grid.position[1];
  *z = self->grid.position[2];
}

void
rvl_get_data_buffer (RVL *self, RVLByte **buffer)
{
  *buffer = self->data.rbuf;
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

RVLSize
rvl_get_voxel_dimensions_byte_count (RVL *self)
{
  RVLSize numVoxelDimen;

  switch (self->grid.type)
    {
    case RVLGridType_Cartesian:
      numVoxelDimen = 3;
      break;
    case RVLGridType_Regular:
      numVoxelDimen = 3;
      break;
    case RVLGridType_Rectilinear:
      {
        const u32 *res = self->resolution;
        numVoxelDimen  = res[0] + res[1] + res[2];
        break;
      }
    }

  return numVoxelDimen * sizeof (f32);
}

RVLSize
rvl_get_data_byte_count (RVL *self)
{
  const u32 *res = self->resolution;
  return res[0] * res[1] * res[2] * rvl_get_primitive_byte_count (self);
}
