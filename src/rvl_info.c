#include <stdlib.h>
#include <string.h>

#include "detail/rvl_info_p.h"
#include "detail/rvl_p.h"

RVLInfo *
rvl_info_create ()
{
  RVLInfo *self = (RVLInfo *)malloc (sizeof (RVLInfo));
  memset (self, 0, sizeof (RVLInfo));
  return self;
}

void
rvl_info_destroy (RVLInfo **self)
{
  if (*self == NULL)
    {
      return;
    }
  free (*self);
  *self = NULL;
}

void
rvl_info_set_grid (RVLInfo *self, RVLGridType gridType, RVLGridUnit gridUnit)
{
  self->gridType = gridType;
  self->gridUnit = gridUnit;
}

void
rvl_info_set_data_form (RVLInfo *self, RVLDataFormat format,
                        RVLBitDepth bitDepth, RVLDataDimen dimen)
{
  self->dataFormat = format;
  self->bitDepth = bitDepth;
  self->dataDimen = dimen;
}

void
rvl_info_set_endian (RVLInfo *self, RVLEndian endian)
{
  self->endian = endian;
}

void
rvl_info_set_resolution (RVLInfo *self, int x, int y, int z)
{
  self->resolution[0] = x;
  self->resolution[1] = y;
  self->resolution[2] = z;
}

void
rvl_info_set_voxel_size (RVLInfo *self, float x, float y, float z)
{
  self->voxelSize[0] = x;
  self->voxelSize[1] = y;
  self->voxelSize[2] = z;
}

void
rvl_info_get_grid (RVLInfo *self, RVLGridType *type, RVLGridUnit *unit)
{
  *type = self->gridType;
  *unit = self->gridUnit;
}

void
rvl_info_get_data_form (RVLInfo *self, RVLDataFormat *format,
                        RVLBitDepth *bits, RVLDataDimen *dimen)
{
  *format = self->dataFormat;
  *bits = self->bitDepth;
  *dimen = self->dataDimen;
}

void
rvl_info_get_endian (RVLInfo *self, RVLEndian *endian)
{
  *endian = self->endian;
}

void
rvl_info_get_resolution (RVLInfo *self, int *x, int *y, int *z)
{
  *x = self->resolution[0];
  *y = self->resolution[1];
  *z = self->resolution[2];
}

void
rvl_info_get_voxel_size (RVLInfo *self, float *x, float *y, float *z)
{
  *x = self->voxelSize[0];
  *y = self->voxelSize[1];
  *z = self->voxelSize[2];
}

void
rvl_info_get_position (RVLInfo *self, float *x, float *y, float *z)
{
  *x = self->position[0];
  *y = self->position[1];
  *z = self->position[2];
}
