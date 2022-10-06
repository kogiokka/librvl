#include <stdlib.h>
#include <string.h>

#include "detail/rvl_info_p.h"
#include "detail/rvl_p.h"

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
rvl_info_set_resolution (RVLInfo_t *self, int x, int y, int z)
{
  self->resolution[0] = x;
  self->resolution[1] = y;
  self->resolution[2] = z;
}

void
rvl_info_set_voxel_size (RVLInfo_t *self, float x, float y, float z)
{
  self->voxelSize[0] = x;
  self->voxelSize[1] = y;
  self->voxelSize[2] = z;
}

void
rvl_info_get_grid (RVLInfo_t *self, RVLGridType_t *type, RVLGridUnit_t *unit)
{
  *type = self->gridType;
  *unit = self->gridUnit;
}

void
rvl_info_get_data_form (RVLInfo_t *self, RVLDataFormat_t *format,
                        RVLBitDepth_t *bits, RVLDataDimen_t *dimen)
{
  *format = self->dataFormat;
  *bits = self->bitDepth;
  *dimen = self->dataDimen;
}

void
rvl_info_get_endian (RVLInfo_t *self, RVLEndian_t *endian)
{
  *endian = self->endian;
}

void
rvl_info_get_resolution (RVLInfo_t *self, int *x, int *y, int *z)
{
  *x = self->resolution[0];
  *y = self->resolution[1];
  *z = self->resolution[2];
}

void
rvl_info_get_voxel_size (RVLInfo_t *self, float *x, float *y, float *z)
{
  *x = self->voxelSize[0];
  *y = self->voxelSize[1];
  *z = self->voxelSize[2];
}

void
rvl_info_get_position (RVLInfo_t *self, float *x, float *y, float *z)
{
  *x = self->position[0];
  *y = self->position[1];
  *z = self->position[2];
}
