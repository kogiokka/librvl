#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
  memset (&rvl->data, 0, sizeof (RVLData));
  rvl->writeData = rvl_write_data_default;
  return rvl;
}

RVL *
rvl_create_reader (const char *filename)
{
  RVL *rvl = rvl_create (filename, RVLIoState_Read);
  memset (&rvl->data, 0, sizeof (RVLData));
  rvl->readData = rvl_read_data_default;
  return rvl;
}

void
rvl_destroy (RVL **self)
{
  RVL *ptr = *self;
  rvl_text_destroy_array (&ptr->text);

  // Dealloc read buffer. Writer buffer pointer is non-owning so the user is
  // responsible for calling this dealloc function.
  rvl_dealloc_data_buffer (ptr, &ptr->data.rbuf);
  rvl_dealloc (ptr, &ptr->grid.vxDimBuf);

  fclose (ptr->io);
  free (ptr);

  *self = NULL;
}

void
rvl_alloc (RVL *self, RVLByte **ptr, RVLSize size)
{
  assert (self != NULL || ptr != NULL);

  if (*ptr != NULL)
    {
      free (*ptr);
    }

  *ptr = (RVLByte *)calloc (1, size);

  if (*ptr == NULL)
    {
      fprintf (stderr, "[librvl] Memory allocation failure.\n");
      exit (EXIT_FAILURE);
    }
}

void
rvl_dealloc (RVL *self, RVLByte **ptr)
{
  assert (self != NULL || ptr != NULL);

  if (*ptr == NULL)
    return;

  free (*ptr);
  *ptr = NULL;
}

RVL *
rvl_create (const char *filename, RVLIoState ioState)
{
  RVL *self        = (RVL *)malloc (sizeof (RVL));
  self->version[0] = RVL_VERSION_MAJOR;
  self->version[1] = RVL_VERSION_MINOR;
  self->ioState    = ioState;
  self->text       = NULL;
  self->numText    = 0;

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
rvl_alloc_data_buffer (RVL *self, RVLByte **buffer, RVLSize *size)
{
  *size = rvl_get_data_byte_count (self);
  rvl_alloc (self, buffer, *size);
}

void
rvl_dealloc_data_buffer (RVL *self, RVLByte **buffer)
{
  rvl_dealloc (self, buffer);
}
