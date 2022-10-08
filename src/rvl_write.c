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
  rvl_write_INFO_chunk (self);

  if (self->text != NULL)
    {
      rvl_write_TEXT_chunk (self, self->text, self->numText);
    }

  rvl_write_DATA_chunk (self);
  rvl_write_END_chunk (self);
}
