#include <stdlib.h>

#include <log.h>

#include "rvl.h"

#include "detail/rvl_p.h"

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

RVLenum
rvl_get_primitive (RVL *self)
{
  return self->primitive;
}

RVLenum
rvl_get_endian (RVL *self)
{
  return self->endian;
}

void
rvl_get_resolution (RVL *self, int *x, int *y, int *z)
{
  *x = self->resolution[0];
  *y = self->resolution[1];
  *z = self->resolution[2];
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
rvl_get_grid_position (RVL *self, float *x, float *y, float *z)
{
  *x = self->grid.position[0];
  *y = self->grid.position[1];
  *z = self->grid.position[2];
}

void
rvl_get_data_buffer (RVL *self, unsigned char **buffer)
{
  *buffer = (unsigned char *)self->data.rbuf;
}

void
rvl_get_text (RVL *self, RVLText **text, int *numText)
{
  *text    = self->text;
  *numText = self->numText;
}

int
rvl_get_primitive_nbytes (RVL *self)
{
  BYTE *p = (BYTE *)&self->primitive;

  u8 dimen = p[1];
  u8 bytes = (1 << (p[0] & 0x0f)) / 8;

  u32 nbytes = dimen * bytes;

  if (nbytes <= 0)
    {
      log_error ("[librvl get] Invalid primitive: %.4x", self->primitive);
    }

  return nbytes;
}

int
rvl_get_data_nbytes (RVL *self)
{
  const u32 *res = self->resolution;

  if (res[0] <= 0 || res[1] <= 0 || res[2] <= 0)
    {
      log_error ("[librvl get] Invalid resolution: %d, %d, %d", res[0], res[1],
                 res[2]);
    }

  return res[0] * res[1] * res[2] * rvl_get_primitive_nbytes (self);
}
