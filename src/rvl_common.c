#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_p.h"
#include "detail/rvl_read_p.h"
#include "detail/rvl_write_p.h"

// .RVL FORMAT\0
RVLByte RVL_FILE_SIG[RVL_FILE_SIG_SIZE] = {
  131, 82, 86, 76, 32, 70, 79, 82, 77, 65, 84, 0,
};

static RVL *rvl_create (const char *filename, RVLIoState ioState);

RVL *
rvl_create_writer (const char *filename)
{
  RVL *rvl = rvl_create (filename, RVLIoState_Write);
  rvl->writeData = rvl_write_data_default;
  return rvl;
}

RVL *
rvl_create_reader (const char *filename)
{
  RVL *rvl = rvl_create (filename, RVLIoState_Read);
  rvl->readData = rvl_read_data_default;
  return rvl;
}

void
rvl_destroy (RVL **self)
{
  RVL *ptr = *self;
  rvl_info_destroy (&ptr->info);
  rvl_text_array_destroy (&ptr->text);
  rvl_data_destroy (&ptr->data);

  fclose (ptr->io);
  free (ptr);

  *self = NULL;
}

RVL *
rvl_create (const char *filename, RVLIoState ioState)
{
  RVL *self = (RVL *)malloc (sizeof (RVL));
  self->version[0] = RVL_VERSION_MAJOR;
  self->version[1] = RVL_VERSION_MINOR;
  self->ioState = ioState;
  self->info = NULL;
  self->data = NULL;
  self->text = NULL;
  self->numText = 0;

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
