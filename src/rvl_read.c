#include <stdio.h>
#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_read_p.h"

void
rvl_read (RVL *self)
{
  if (self == NULL)
    return;

  rvl_read_file_sig (self);

  RVLChunkCode_t code;
  do
    {
      RVLSize size;
      rvl_read_chunk_header (self, &size, &code);
#ifndef NDEBUG
      char *ch = (char *)&code;
      printf ("# Reading header\n"
              "    size: %d\n"
              "    code: %c%c%c%c\n",
              size, ch[0], ch[1], ch[2], ch[3]);
#endif

      RVLByte *buffer = (RVLByte *)malloc (size);
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
            RVLConstByte *ch = (RVLConstByte *)&code;
            fprintf (stderr, "Unknown chunk code: %c%c%c%c", ch[0], ch[1],
                     ch[2], ch[3]);
          }
          break;
        }
      free (buffer);
    }
  while (code != RVLChunkCode_END);
}
