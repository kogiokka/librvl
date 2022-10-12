#include <stdio.h>
#include <stdlib.h>

#include "rvl.h"

#include "detail/rvl_read_p.h"

void
rvl_read_rvl (RVL *self)
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

      RVLByte *buffer = rvl_alloc (self, size);
      rvl_read_chunk_payload (self, buffer, size);

      switch (code)
        {
        case RVLChunkCode_INFO:
          rvl_read_INFO_chunk (self, buffer, size);
          break;
        case RVLChunkCode_DATA:
          rvl_alloc_data_buffer (self, &self->data.rbuf, &self->data.size);
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
      rvl_dealloc (self, &buffer);
    }
  while (code != RVLChunkCode_END);
}

void
rvl_read_info (RVL *self)
{
  if (self == NULL)
    return;

  rvl_read_file_sig (self);

  RVLChunkCode_t code;
  do
    {
      RVLSize size;
      rvl_read_chunk_header (self, &size, &code);

      if (code == RVLChunkCode_INFO)
        {
          RVLByte *buffer = rvl_alloc (self, size);
          rvl_read_chunk_payload (self, buffer, size);
          rvl_read_INFO_chunk (self, buffer, size);
          rvl_dealloc (self, &buffer);
          break;
        }
    }
  while (code != RVLChunkCode_END);
}

void
rvl_read_data_buffer (RVL *self, RVLByte **data)
{
  RVLChunkCode_t code;
  self->data.rbuf = *data;
  do
    {
      RVLSize size;
      rvl_read_chunk_header (self, &size, &code);
      if (code == RVLChunkCode_DATA)
        {
          RVLByte *buffer = rvl_alloc (self, size);
          rvl_read_chunk_payload (self, buffer, size);
          rvl_read_DATA_chunk (self, buffer, size);
          rvl_dealloc (self, &buffer);
          break;
        }
    }
  while (code != RVLChunkCode_END);
  self->data.rbuf = NULL;
}
