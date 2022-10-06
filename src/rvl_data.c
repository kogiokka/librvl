#include <stdlib.h>
#include <string.h>

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"

RVLData_t *
rvl_data_create ()
{
  RVLData_t *data = (RVLData_t *)malloc (sizeof (RVLData_t));
  data->buffer = NULL;
  data->size = 0;

  return data;
}

void
rvl_data_alloc (RVLData_t *self, RVLInfo_t *info)
{
  if (self->buffer != NULL)
    {
      free (self->buffer);
    }

  const u32 size
      = info->resolution.x * info->resolution.y * info->resolution.z;
  self->buffer = (u8 *)malloc (sizeof (u8) * size);
  self->size = size;
}

void
rvl_data_destroy (RVLData_t **self)
{
  if (*self == NULL)
    {
      return;
    }
  if ((*self)->buffer != NULL)
    {
      free ((*self)->buffer);
      (*self)->buffer = NULL;
    }
  free (*self);
  *self = NULL;
}

void
rvl_data_set (RVLData_t *self, uint8_t *data, uint32_t size, uint32_t offset)
{
  memcpy (&self->buffer[offset], data, size);
}

uint32_t
rvl_data_get_buffer (RVLData_t *self, uint8_t **buffer)
{
  *buffer = self->buffer;
  return self->size;
}
