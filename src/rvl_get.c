#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_p.h"
#include "detail/rvl_text_p.h"
#include "util/logger.h"

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

char *
rvl_get_voxels (RVL *self)
{
  return (char *)self->data.rbuf;
}

void *
rvl_get_voxel_at (RVL *self, int x, int y, int z)
{
  u32 nx     = self->resolution[0];
  u32 ny     = self->resolution[1];
  u32 index  = (x + y * nx + z * nx * ny);
  u32 offset = index * rvl_sizeof (self->primitive);

  return (void *)(self->data.rbuf + offset);
}

const char *
rvl_get_text_value (RVL *self, RVLenum tag)
{
  RVLText *cur = self->text;
  while (cur != NULL)
    {
      if (cur->tag == tag)
        {
          log_debug ("Get TEXT: %.4X, %s", cur->tag, cur->value);
          return cur->value;
        }
      cur = cur->next;
    }

  return "";
}
