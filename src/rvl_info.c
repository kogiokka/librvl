#include <stdlib.h>
#include <string.h>

#include "detail/rvl_p.h"
#include "detail/rvl_info_p.h"

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
  self->grid.type = gridType;
  self->grid.unit = gridUnit;
}

void
rvl_info_set_data_form (RVLInfo_t *self, RVLDataFormat_t format,
                        RVLBitDepth_t bitDepth, RVLDataDimen_t dimen)
{
  self->dataForm.format = format;
  self->dataForm.bits = bitDepth;
  self->dataForm.dimen = dimen;
}

void
rvl_info_set_endian (RVLInfo_t *self, RVLEndian_t endian)
{
  self->endian = endian;
}

void
rvl_info_set_resolution (RVLInfo_t *self, u32 x, u32 y, u32 z)
{
  self->resolution.x = x;
  self->resolution.y = y;
  self->resolution.z = z;
}

void
rvl_info_set_voxel_size (RVLInfo_t *self, f32 x, f32 y, f32 z)
{
  self->voxelSize.x = x;
  self->voxelSize.y = y;
  self->voxelSize.z = z;
}

RVLInfoGrid_t
rvl_info_get_grid (RVLInfo_t *self)
{
  RVLInfoGrid_t grid;
  grid = self->grid;
  return grid;
}

RVLInfoDataForm_t
rvl_info_get_data_form (RVLInfo_t *self)
{
  RVLInfoDataForm_t form;
  form = self->dataForm;
  return form;
}

RVLEndian_t
rvl_info_get_endian (RVLInfo_t *self)
{
  return self->endian;
}

RVLVec3u_t
rvl_info_get_resolution (RVLInfo_t *self)
{
  return self->resolution;
}

RVLVec3f_t
rvl_info_get_voxel_size (RVLInfo_t *self)
{
  return self->voxelSize;
}

RVLVec3f_t
rvl_info_get_position (RVLInfo_t *self)
{
  return self->position;
}
