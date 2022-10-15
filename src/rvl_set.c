#include <stdlib.h>

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
rvl_set_voxel_dimensions (RVL *self, const float *dimensions)
{
  self->grid.vxDimData.wbuf = (RVLConstByte *)dimensions;
  self->grid.vxDimData.size = rvl_get_voxel_dimensions_byte_count (self);
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
