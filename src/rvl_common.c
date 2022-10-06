#include <stdlib.h>

#include "rvl.h"
#include "detail/rvl_p.h"

RVL_t *
rvl_create (const char *filename, RVLIoState_t ioState)
{
  RVL_t *self = (RVL_t *)malloc (sizeof (RVL_t));
  self->version[0] = RVL_VERSION_MAJOR;
  self->version[1] = RVL_VERSION_MINOR;
  self->ioState = ioState;

  switch (ioState)
    {
    case RVLIoState_Read:
      self->io = fopen (filename, "rb");
      break;
    case RVLIoState_Write:
      self->io = fopen (filename, "wb");
      break;
    }

  return self;
}

void
rvl_destroy (RVL_t **self)
{
  RVL_t *ptr = *self;
  rvl_info_destroy (&ptr->info);
  rvl_text_array_destroy (&ptr->text);
  rvl_data_destroy (&ptr->data);

  fclose (ptr->io);
  free (ptr);

  *self = NULL;
}
