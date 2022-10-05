#include <lz4.h>
#include <lz4hc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rvl.h"

#define SIG_SIZE 12

static void rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code,
                                    RVLSize_t size);
static void rvl_write_chunk_payload (RVL_t *self, RVLConstByte_t *data,
                                     RVLSize_t size);
static void rvl_write_chunk_end (RVL_t *self);
static void rvl_write_file (RVL_t *self, RVLConstByte_t *data, RVLSize_t size);

void
rvl_write_file_sig (RVL_t *self)
{
  // .RVL FORMAT\0
  RVLByte_t signature[SIG_SIZE] = {
    131, 82, 86, 76, 32, 70, 79, 82, 77, 65, 84, 0,
  };

  rvl_write_file (self, signature, SIG_SIZE);
}

void
rvl_write_INFO (RVL_t *self, const RVLInfo_t *info)
{
  RVLSize_t byteSize = 44;
  RVLByte_t *buf = calloc (1, byteSize);

  memcpy (&buf[0], info->version, 2);
  buf[2] = info->gridType;
  buf[3] = info->gridUnit;
  buf[4] = info->dataDimen;
  buf[5] = info->bitDepth;
  buf[6] = info->endian;
  memcpy (&buf[7], info->resolution, 12);
  memcpy (&buf[19], info->voxelSize, 12);
  memcpy (&buf[31], info->originCoord, 12);

  rvl_write_chunk_header (self, RVLChunkCode_INFO, byteSize);
  rvl_write_chunk_payload (self, buf, byteSize);
  rvl_write_chunk_end (self);
}

void
rvl_write_DATA (RVL_t *self, RVLConstByte_t *data, RVLSize_t size)
{
  char *compressedBuf = (char *)malloc (size);
  const int compressedSize = LZ4_compress_HC ((char *)data, compressedBuf,
                                              size, size, LZ4HC_CLEVEL_MIN);

  rvl_write_chunk_header (self, RVLChunkCode_DATA, compressedSize);
  rvl_write_chunk_payload (self, (RVLConstByte_t *)compressedBuf,
                           compressedSize);
  rvl_write_chunk_end (self);
}

void
rvl_write_TEXT (RVL_t *self, const RVLText_t *arrText, int numText)
{
  for (int i = 0; i < numText; i++)
    {
      const RVLText_t *const text = &arrText[i];

      char keyword[80];
      RVLSize_t keySize = strlen (text->key);
      RVLSize_t valueSize = strlen (text->value);

      if (keySize >= 80)
        {
          fprintf (
              stderr,
              "[WARNING] Keyword length exceeds the maximum limit and will "
              "be truncated:\n \"%s\"\n",
              text->key);
          keySize = 79;
        }

      rvl_write_chunk_header (self, RVLChunkCode_TEXT,
                              keySize + valueSize + 1);

      // Chunk Payload
      memcpy (keyword, text->key, keySize);
      keyword[keySize] = '\0';
      rvl_write_chunk_payload (self, (RVLConstByte_t *)keyword, keySize + 1);

      if (valueSize != 0)
        {
          rvl_write_chunk_payload (self, (RVLConstByte_t *)text->value,
                                   valueSize);
        }

      rvl_write_chunk_end (self);
    }
}

void
rvl_write_END (RVL_t *self)
{
  rvl_write_chunk_header (self, RVLChunkCode_END, 0);
  rvl_write_chunk_payload (self, NULL, 0);
  rvl_write_chunk_end (self);
}

void
rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code, RVLSize_t size)
{
  RVLByte_t buf[8];

  // Chunk Size(bytes)
  memcpy (buf, &size, 4);
  // Chunk Code
  memcpy (&buf[4], &code, 4);

  rvl_write_file (self, buf, 8);
}

void
rvl_write_chunk_payload (RVL_t *self, RVLConstByte_t *data, RVLSize_t size)
{
  if (data != NULL && size > 0)
    {
      rvl_write_file (self, data, size);
    }
}

void
rvl_write_chunk_end (RVL_t *self)
{
  return;
}

void
rvl_write_file (RVL_t *self, RVLConstByte_t *data, RVLSize_t size)
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
