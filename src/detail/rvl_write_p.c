#include <lz4.h>
#include <lz4hc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "detail/rvl_write_p.h"

#include "detail/rvl_text_p.h"

void
rvl_write_VHDR_chunk (RVL *self)
{
  u32 byteSize = 44;
  u8 *buf      = calloc (1, byteSize);

  memcpy (&buf[0], self->version, 2);
  buf[2] = self->gridType;
  buf[3] = self->gridUnit;
  memcpy (&buf[4], &self->primitive, 2);
  buf[6] = self->endian;
  buf[7] = 0; // padding
  memcpy (&buf[8], &self->resolution[0], 12);
  memcpy (&buf[20], &self->voxelSize[0], 12);
  memcpy (&buf[32], &self->position[0], 12);

  rvl_write_chunk_header (self, RVLChunkCode_VHDR, byteSize);
  rvl_write_chunk_payload (self, buf, byteSize);
  rvl_write_chunk_end (self);
}

void
rvl_write_DATA_chunk (RVL *self)
{
  int         srcSize     = self->data.size;
  int         dstCapacity = self->data.size;
  const char *src         = (char *)self->data.wbuf;
  char       *dst         = (char *)malloc (dstCapacity);

  int compSize
      = LZ4_compress_HC (src, dst, srcSize, dstCapacity, LZ4HC_CLEVEL_MIN);

  // When the compressed size is greater than the uncompressed one.
  if (compSize == 0)
    {
      dstCapacity = LZ4_compressBound (self->data.size);
      free (dst);
      dst = malloc (dstCapacity);

      compSize = LZ4_compress_HC (src, dst, self->data.size, dstCapacity,
                                  LZ4HC_CLEVEL_MIN);
    }

  rvl_write_chunk_header (self, RVLChunkCode_DATA, compSize);
  rvl_write_chunk_payload (self, (RVLConstByte *)dst, compSize);
  rvl_write_chunk_end (self);
}

// Strip off the null terminator at the end of the value string.
void
rvl_write_TEXT_chunk (RVL *self, const RVLText *textArr, int numText)
{
  for (int i = 0; i < numText; i++)
    {
      const RVLText *const text      = &textArr[i];
      const RVLSize        keySize   = strlen (text->key);
      const RVLSize        valueSize = strlen (text->value);

      rvl_write_chunk_header (self, RVLChunkCode_TEXT,
                              keySize + valueSize + 1);

      // Include the null terminator
      rvl_write_chunk_payload (self, (RVLConstByte *)text->key, keySize + 1);

      if (valueSize != 0)
        {
          rvl_write_chunk_payload (self, (RVLConstByte *)text->value,
                                   valueSize);
        }

      rvl_write_chunk_end (self);
    }
}

void
rvl_write_VEND_chunk (RVL *self)
{
  rvl_write_chunk_header (self, RVLChunkCode_VEND, 0);
  rvl_write_chunk_payload (self, NULL, 0);
  rvl_write_chunk_end (self);
}

void
rvl_write_chunk_header (RVL *self, RVLChunkCode code, RVLSize size)
{
  u8 buf[8];

  // Chunk Size(bytes)
  memcpy (buf, &size, 4);
  // Chunk Code
  memcpy (&buf[4], &code, 4);

  rvl_write_data (self, buf, 8);
}

void
rvl_write_chunk_payload (RVL *self, RVLConstByte *data, RVLSize size)
{
  if (data != NULL && size > 0)
    {
      rvl_write_data (self, data, size);
    }
}

void
rvl_write_chunk_end (RVL *self)
{
  return;
}

void
rvl_write_file_sig (RVL *self)
{
  rvl_write_data (self, RVL_FILE_SIG, RVL_FILE_SIG_SIZE);
}

void
rvl_write_data (RVL *self, RVLConstByte *data, RVLSize size)
{
  if (self->writeData == NULL)
    {
      fprintf (
          stderr,
          "[ERROR] The write data function is NULL. Might be due to wrong "
          "read/write instance of RVL_t.\n");
      exit (EXIT_FAILURE);
    }

  self->writeData (self, data, size);
}

void
rvl_write_data_default (RVL *self, RVLConstByte *data, RVLSize size)
{
  if (self->io == NULL)
    {
      fprintf (stderr, "[ERROR] RVL_t is not initialized!");
      exit (EXIT_FAILURE);
    }

  const size_t count = fwrite (data, 1, size, self->io);

  if (count != size)
    {
      fprintf (stderr, "[ERROR] Write error occured!");
      exit (EXIT_FAILURE);
    }
}
