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
rvl_set_position (RVL *self, float x, float y, float z)
{
  self->grid.position[0] = x;
  self->grid.position[1] = y;
  self->grid.position[2] = z;
}

void
rvl_set_voxel_dimensions (RVL *self, float x, float y, float z)
{
  RVLSize size = rvl_get_voxel_dimensions_byte_count (self);

  if (self->grid.vxDimBuf != NULL)
    {
      free (self->grid.vxDimBuf);
    }

  self->grid.vxDimBuf     = rvl_alloc (self, size);
  self->grid.vxDimBufSize = size;

  f32 arr[3] = { x, y, z };
  memcpy (self->grid.vxDimBuf, arr, size);
}

void
rvl_set_voxel_dimensions_v (RVL *self, int n, const float *dimensions)
{
  RVLSize size = n * sizeof (f32);

  if (self->grid.vxDimBuf != NULL)
    {
      free (self->grid.vxDimBuf);
    }

  self->grid.vxDimBuf     = rvl_alloc (self, size);
  self->grid.vxDimBufSize = size;
  memcpy (self->grid.vxDimBuf, dimensions, size);
}

void
rvl_set_data_buffer (RVL *self, RVLConstByte *buffer, RVLSize size)
{
  self->data.wbuf = buffer;
  self->data.size = size;
}

void
rvl_set_text (RVL *self, RVLText **text, int numText)
{
  if (self->text != NULL)
    {
      self->numText = 0;
      rvl_text_destroy_array (&self->text);
    }

  self->text    = *text;
  self->numText = numText;
  *text         = NULL;
}
