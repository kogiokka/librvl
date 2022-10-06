#include <stdlib.h>
#include <string.h>

#include "rvl_data.h"

RVLData_t *
rvl_data_create (RVLInfo_t *info)
{
  RVLData_t *rvldata = (RVLData_t *)malloc (sizeof (RVLData_t));
  const u32 size
      = info->resolution[0] * info->resolution[1] * info->resolution[2];
  rvldata->buffer = (u8 *)malloc (sizeof (u8) * size);
  rvldata->size = size;

  return rvldata;
}

void
rvl_data_destroy (RVLData_t **self)
{
  if (*self == NULL)
    {
      return;
    }
  free (*self);
  *self = NULL;
}
