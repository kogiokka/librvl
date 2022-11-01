#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_log_p.h"
#include "detail/rvl_p.h"
#include "detail/rvl_text_p.h"

void
rvl_get_volumetric_format (RVL *self, int *nx, int *ny, int *nz,
                           RVLenum *primitive, RVLenum *endian)
{
  *nx        = self->resolution[0];
  *ny        = self->resolution[1];
  *nz        = self->resolution[2];
  *primitive = self->primitive;
  *endian    = self->endian;
}

RVLenum
rvl_get_compression (RVL *self)
{
  return self->compress;
}

RVLenum
rvl_get_grid_unit (RVL *self)
{
  return self->grid.unit;
}

RVLenum
rvl_get_grid_type (RVL *self)
{
  return self->grid.type;
}

void
rvl_get_voxel_dims (RVL *self, float *dx, float *dy, float *dz)
{
  *dx = ((float *)self->grid.dx)[0];
  *dy = ((float *)self->grid.dy)[0];
  *dz = ((float *)self->grid.dz)[0];
}

void
rvl_get_voxel_dims_v (RVL *self, int *ndx, int *ndy, int *ndz,
                      const float **dx, const float **dy, const float **dz)
{
  *ndx = self->grid.ndx;
  *ndy = self->grid.ndy;
  *ndz = self->grid.ndz;
  *dx  = (const float *)self->grid.dx;
  *dy  = (const float *)self->grid.dy;
  *dz  = (const float *)self->grid.dz;
}

void
rvl_get_grid_origin (RVL *self, float *x, float *y, float *z)
{
  *x = self->grid.position[0];
  *y = self->grid.position[1];
  *z = self->grid.position[2];
}

void
rvl_get_voxels (RVL *self, const void **voxels)
{
  *voxels = (unsigned char *)self->data.rbuf;
}

void
rvl_get_data_buffer (RVL *self, const void **buffer)
{
  rvl_get_voxels (self, buffer);
}

void
rvl_get_text (RVL *self, RVLenum tag, const char **value)
{
  RVLText *cur = self->text;
  while (cur != NULL)
    {
      if (cur->tag == tag)
        {
          rvl_log_debug ("Get TEXT: %.4X, %s", cur->tag, cur->value);
          *value = cur->value;
          return;
        }
      cur = cur->next;
    }

  *value = "";
}

unsigned int
rvl_get_primitive_nbytes (RVL *self)
{
  return rvl_eval_primitive_nbyte (self);
}

unsigned int
rvl_get_data_nbytes (RVL *self)
{
  return rvl_eval_voxels_nbyte (self);
}

void
rvl_get_grid_position (RVL *self, float *x, float *y, float *z)
{
  rvl_get_grid_origin (self, x, y, z);
}
