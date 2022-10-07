#include <lz4.h>
#include <lz4hc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "detail/rvl_p.h"

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"
#include "detail/rvl_text_p.h"

static void rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code,
                                    rvlsize_t size);
static void rvl_write_chunk_payload (RVL_t *self, rvlcbyte_t *data,
                                     rvlsize_t size);
static void rvl_write_chunk_end (RVL_t *self);
static void rvl_write_data (RVL_t *self, rvlcbyte_t *data, rvlsize_t size);

static void rvl_write_file_sig (RVL_t *self);
static void rvl_write_INFO_chunk (RVL_t *self, const RVLInfo_t *info);
static void rvl_write_DATA_chunk (RVL_t *self, const RVLData_t *data);
static void rvl_write_TEXT_chunk (RVL_t *self, const RVLText_t *textArr,
                                  int numText);
static void rvl_write_END_chunk (RVL_t *self);

void
rvl_set_INFO (RVL_t *self, RVLInfo_t **info)
{
  if (self->info != NULL)
    {
      rvl_info_destroy (&self->info);
    }

  self->info = *info;
  *info = NULL;
}

void
rvl_set_TEXT (RVL_t *self, RVLText_t **text, int numText)
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
rvl_set_DATA (RVL_t *self, RVLData_t **data)
{
  if (self->data != NULL)
    {
      rvl_data_destroy (&self->data);
    }

  self->data = *data;
  *data = NULL;
}

void
rvl_write (RVL_t *self)
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
rvl_write_file_sig (RVL_t *self)
{
  rvl_write_data (self, RVL_FILE_SIG, RVL_FILE_SIG_SIZE);
}

void
rvl_write_INFO_chunk (RVL_t *self, const RVLInfo_t *info)
{
  u32 byteSize = 44;
  u8 *buf = calloc (1, byteSize);

  memcpy (&buf[0], self->version, 2);
  buf[2] = info->gridType;
  buf[3] = info->gridUnit;
  buf[4] = info->dataFormat;
  buf[5] = info->bitDepth;
  buf[6] = info->dataDimen;
  buf[7] = info->endian;
  memcpy (&buf[8], &info->resolution[0], 12);
  memcpy (&buf[20], &info->voxelSize[0], 12);
  memcpy (&buf[32], &info->position[0], 12);

  rvl_write_chunk_header (self, RVLChunkCode_INFO, byteSize);
  rvl_write_chunk_payload (self, buf, byteSize);
  rvl_write_chunk_end (self);
}

void
rvl_write_DATA_chunk (RVL_t *self, const RVLData_t *data)
{
  char *compressedBuf = (char *)malloc (data->size);
  const int compressedSize
      = LZ4_compress_HC ((char *)data->buffer, compressedBuf, data->size,
                         data->size, LZ4HC_CLEVEL_MIN);

  rvl_write_chunk_header (self, RVLChunkCode_DATA, compressedSize);
  rvl_write_chunk_payload (self, (rvlcbyte_t *)compressedBuf, compressedSize);
  rvl_write_chunk_end (self);
}

// Strip off the null terminator at the end of the value string.
void
rvl_write_TEXT_chunk (RVL_t *self, const RVLText_t *textArr, int numText)
{
  for (int i = 0; i < numText; i++)
    {
      const RVLText_t *const text = &textArr[i];
      const rvlsize_t keySize = strlen (text->key);
      const rvlsize_t valueSize = strlen (text->value);

      rvl_write_chunk_header (self, RVLChunkCode_TEXT,
                              keySize + valueSize + 1);

      // Include the null terminator
      rvl_write_chunk_payload (self, (rvlcbyte_t *)text->key, keySize + 1);

      if (valueSize != 0)
        {
          rvl_write_chunk_payload (self, (rvlcbyte_t *)text->value, valueSize);
        }

      rvl_write_chunk_end (self);
    }
}

void
rvl_write_END_chunk (RVL_t *self)
{
  rvl_write_chunk_header (self, RVLChunkCode_END, 0);
  rvl_write_chunk_payload (self, NULL, 0);
  rvl_write_chunk_end (self);
}

void
rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code, rvlsize_t size)
{
  u8 buf[8];

  // Chunk Size(bytes)
  memcpy (buf, &size, 4);
  // Chunk Code
  memcpy (&buf[4], &code, 4);

  rvl_write_data (self, buf, 8);
}

void
rvl_write_chunk_payload (RVL_t *self, rvlcbyte_t *data, rvlsize_t size)
{
  if (data != NULL && size > 0)
    {
      rvl_write_data (self, data, size);
    }
}

void
rvl_write_chunk_end (RVL_t *self)
{
  return;
}

void
rvl_write_data (RVL_t *self, rvlcbyte_t *data, rvlsize_t size)
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
