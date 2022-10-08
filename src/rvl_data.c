#include <stdlib.h>
#include <string.h>

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"

RVLData *
rvl_data_create ()
{
  RVLData *data = (RVLData *)malloc (sizeof (RVLData));
  data->buffer = NULL;
  data->size = 0;

  return data;
}

void
rvl_data_alloc (RVLData *self, RVLInfo *info)
{
  if (self->buffer != NULL)
    {
      free (self->buffer);
    }

  const u32 *res = info->resolution;
  const RVLSize size = res[0] * res[1] * res[2];

  self->buffer = (RVLByte *)malloc (size);
  self->size = size;
}

void
rvl_data_destroy (RVLData **self)
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
rvl_data_set_sub_range (RVLData *self, RVLSize offset, RVLSize size,
                        RVLByte *data)
{
  memcpy (self->buffer + offset, data, size);
}

RVLSize
rvl_data_get_buffer (RVLData *self, RVLByte **buffer)
{
  *buffer = self->buffer;
  return self->size;
}
