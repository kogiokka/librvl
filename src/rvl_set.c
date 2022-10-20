#include <stdlib.h>
#include <string.h>

#include "rvl.h"

#include "detail/rvl_p.h"

void
rvl_set_grid_type (RVL *self, RVLGridType gridType)
{
  self->grid.type = gridType;
}

void
rvl_set_grid_unit (RVL *self, RVLGridUnit gridUnit)
{
  self->grid.unit = gridUnit;
}

void
rvl_set_primitive (RVL *self, RVLPrimitive primitive)
{
  self->primitive = primitive;
}

void
rvl_set_endian (RVL *self, RVLEndian endian)
{
  self->endian = endian;
}

void
rvl_set_resolution (RVL *self, int x, int y, int z)
{
  self->resolution[0] = x;
  self->resolution[1] = y;
  self->resolution[2] = z;
}

void
rvl_set_grid_position (RVL *self, float x, float y, float z)
{
  self->grid.position[0] = x;
  self->grid.position[1] = y;
  self->grid.position[2] = z;
}

void
rvl_set_voxel_dims (RVL *self, float dx, float dy, float dz)
{
  RVLSize size = 1 * sizeof (f32);

  self->grid.ndx  = 1;
  self->grid.ndy  = 1;
  self->grid.ndz  = 1;
  self->grid.dxSz = size;
  self->grid.dySz = size;
  self->grid.dzSz = size;
  self->grid.dx   = (RVLByte *)malloc (size);
  self->grid.dy   = (RVLByte *)malloc (size);
  self->grid.dz   = (RVLByte *)malloc (size);

  ((float*)self->grid.dx)[0] = dx;
  ((float*)self->grid.dy)[0] = dy;
  ((float*)self->grid.dz)[0] = dz;
}

void
rvl_set_voxel_dims_v (RVL *self, int ndx, int ndy, int ndz, float *dx,
                      float *dy, float *dz)
{
  RVLSize sizef32 = sizeof (f32);

  self->grid.ndx  = ndx;
  self->grid.ndy  = ndy;
  self->grid.ndz  = ndz;
  self->grid.dxSz = ndx * sizef32;
  self->grid.dySz = ndy * sizef32;
  self->grid.dzSz = ndz * sizef32;
  self->grid.dx   = (RVLByte *)malloc (self->grid.dxSz);
  self->grid.dy   = (RVLByte *)malloc (self->grid.dySz);
  self->grid.dz   = (RVLByte *)malloc (self->grid.dzSz);

  memcpy (self->grid.dx, dx, self->grid.dxSz);
  memcpy (self->grid.dy, dy, self->grid.dySz);
  memcpy (self->grid.dz, dz, self->grid.dzSz);
}

void
rvl_set_data_buffer (RVL *self, RVLSize size, RVLConstByte *buffer)
{
  self->data.size = size;
  self->data.wbuf = buffer;
}

void
rvl_set_text (RVL *self, RVLText **text, int numText)
{
  rvl_text_destroy_array (&self->text);

  self->text    = *text;
  self->numText = numText;
  *text         = NULL;
}

