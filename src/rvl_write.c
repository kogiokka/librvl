#include <lz4.h>
#include <lz4hc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rvl.h"

#include "detail/rvl_write_p.h"

void
rvl_write (RVL *self)
{
  rvl_write_file_sig (self);

  // Required
  rvl_write_INFO_chunk (self, self->info);

  if (self->text != NULL)
    {
      rvl_write_TEXT_chunk (self, self->text, self->numText);
    }

  if (self->data != NULL)
    {
      rvl_write_DATA_chunk (self, self->data);
    }

  rvl_write_END_chunk (self);
}

void
rvl_set_INFO (RVL *self, RVLInfo **info)
{
  if (self->info != NULL)
    {
      rvl_info_destroy (&self->info);
    }

  self->info = *info;
  *info = NULL;
}

void
rvl_set_TEXT (RVL *self, RVLText **text, int numText)
{
  if (self->text != NULL)
    {
      self->numText = 0;
      rvl_text_array_destroy (&self->text);
    }

  self->text = *text;
  self->numText = numText;
  *text = NULL;
}

void
rvl_set_DATA (RVL *self, RVLData **data)
{
  if (self->data != NULL)
    {
      rvl_data_destroy (&self->data);
    }

  self->data = *data;
  *data = NULL;
}
