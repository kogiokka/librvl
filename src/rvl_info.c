#include <stdlib.h>
#include <string.h>

#include "rvl.h"
#include "rvl_info.h"

RVLInfo_t *
rvl_info_create ()
{
  RVLInfo_t *self = (RVLInfo_t *)malloc (sizeof (RVLInfo_t));
  memset (self, 0, sizeof (RVLInfo_t));
  return self;
}

void
rvl_info_destroy (RVLInfo_t **self)
{
  if (*self == NULL)
    {
      return;
    }
  free (*self);
  *self = NULL;
}

void
rvl_info_set_grid (RVLInfo_t *self, RVLGridType_t gridType,
                   RVLGridUnit_t gridUnit)
{
  self->gridType = gridType;
  self->gridUnit = gridUnit;
}

void
rvl_info_set_data_form (RVLInfo_t *self, RVLDataFormat_t format,
                        RVLBitDepth_t bitDepth, RVLDataDimen_t dimen)
{
  self->dataFormat = format;
  self->bitDepth = bitDepth;
  self->dataDimen = dimen;
}

void
rvl_info_set_endian (RVLInfo_t *self, RVLEndian_t endian)
{
  self->endian = endian;
}

void
rvl_info_set_resolution (RVLInfo_t *self, u32 x, u32 y, u32 z)
{
  self->resolution[0] = x;
  self->resolution[1] = y;
  self->resolution[2] = z;
}

void
rvl_info_set_voxel_size (RVLInfo_t *self, f32 x, f32 y, f32 z)
{
  self->voxelSize[0] = x;
  self->voxelSize[1] = y;
  self->voxelSize[2] = z;
}
