#include <string.h>

#include "rvl.h"

#include "detail/rvl_p.h"

void
rvl_set_grid_type (RVL *self, RVLGridType gridType)
{
  self->gridType = gridType;
}

void
rvl_set_grid_unit (RVL *self, RVLGridUnit gridUnit)
{
  self->gridUnit = gridUnit;
}

void
rvl_set_data_format (RVL *self, RVLDataFormat format)
{
  self->dataFormat = format;
}

void
rvl_set_data_dimensions (RVL *self, RVLDataDimen dimen)
{
  self->dataDimen = dimen;
}

void
rvl_set_data_bit_depth (RVL *self, RVLBitDepth bitDepth)
{
  self->bitDepth = bitDepth;
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
rvl_set_voxel_size (RVL *self, float x, float y, float z)
{
  self->voxelSize[0] = x;
  self->voxelSize[1] = y;
  self->voxelSize[2] = z;
}

void
rvl_set_sub_data_buffer (RVL *self, RVLSize offset, RVLSize size,
                         RVLByte *data)
{
  memcpy (self->data.buffer + offset, data, size);
}

void
rvl_set_text (RVL *self, RVLText **text, int numText)
{
  if (self->text != NULL)
    {
      self->numText = 0;
      rvl_text_destroy_array (&self->text);
    }

  self->text = *text;
  self->numText = numText;
  *text = NULL;
}
