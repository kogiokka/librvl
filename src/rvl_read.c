#include <stdio.h>
#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_read_p.h"

void
rvl_read (RVL_t *self)
{
  if (self == NULL)
    return;

  rvl_read_file_sig (self);

  RVLChunkCode_t code;
  do
    {
      rvlsize_t size;
      rvl_read_chunk_header (self, &size, &code);
#ifndef NDEBUG
      char *ch = (char *)&code;
      printf ("# Reading header\n"
              "    size: %d\n"
              "    code: %c%c%c%c\n",
              size, ch[0], ch[1], ch[2], ch[3]);
#endif

      rvlbyte_t *buffer = (rvlbyte_t *)malloc (size);
      rvl_read_chunk_payload (self, buffer, size);

      switch (code)
        {
        case RVLChunkCode_INFO:
          rvl_read_INFO_chunk (self, buffer, size);
          break;
        case RVLChunkCode_DATA:
          rvl_read_DATA_chunk (self, buffer, size);
          break;
        case RVLChunkCode_TEXT:
          rvl_read_TEXT_chunk (self, buffer, size);
          break;
        case RVLChunkCode_END:
          break;
        default:
          {
            rvlcbyte_t *ch = (rvlcbyte_t *)&code;
            fprintf (stderr, "Unknown chunk code: %c%c%c%c", ch[0], ch[1],
                     ch[2], ch[3]);
          }
          break;
        }
      free (buffer);
    }
  while (code != RVLChunkCode_END);
}

void
rvl_get_INFO (RVL_t *self, RVLInfo_t **info)
{
  *info = self->info;
}

void
rvl_get_DATA (RVL_t *self, RVLData_t **data)
{
  *data = self->data;
}

void
rvl_get_TEXT (RVL_t *self, RVLText_t **text, int *numText)
{
  *text = self->text;
  *numText = self->numText;
}
