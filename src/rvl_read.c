#include <lz4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detail/rvl_p.h"

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"
#include "detail/rvl_text_p.h"

static void rvl_read_data (RVL_t *self, rvlbyte_t *data, rvlsize_t size);
static void rvl_read_chunk_header (RVL_t *self, rvlsize_t *size,
                                   RVLChunkCode_t *code);
static void rvl_read_chunk_payload (RVL_t *self, rvlbyte_t *data,
                                    rvlsize_t size);

static void rvl_read_file_sig (RVL_t *self);
static void rvl_read_INFO_chunk (RVL_t *self, rvlbyte_t *buffer);
static void rvl_read_DATA_chunk (RVL_t *self, rvlbyte_t *buffer,
                                 rvlsize_t size);
static void rvl_read_TEXT_chunk (RVL_t *self, rvlcbyte_t *buffer,
                                 rvlsize_t size);

void
rvl_read_data (RVL_t *self, rvlbyte_t *data, rvlsize_t size)
{
  const size_t count = fread (data, 1, size, self->io);

  if (count != size)
    {
      fprintf (stderr, "[ERROR] Read error occured!");
      exit (EXIT_FAILURE);
    }
}

void
rvl_read_chunk_header (RVL_t *self, rvlsize_t *size, RVLChunkCode_t *code)
{
  rvl_read_data (self, (rvlbyte_t *)size, 4);
  rvl_read_data (self, (rvlbyte_t *)code, 4);
}

void
rvl_read_chunk_payload (RVL_t *self, rvlbyte_t *data, rvlsize_t size)
{
  rvl_read_data (self, data, size);
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
      rvlbyte_t *payload = NULL;
      rvl_read_chunk_header (self, &size, &code);
      rvl_read_chunk_payload (self, payload, size);
      switch (code)
        {
        case RVLChunkCode_INFO:
          rvl_read_INFO_chunk (self, payload);
          break;
        case RVLChunkCode_DATA:
          rvl_read_DATA_chunk (self, payload, size);
          break;
        case RVLChunkCode_TEXT:
          rvl_read_TEXT_chunk (self, payload, size);
          break;
        default:
          fprintf (stderr, "Unknown chunk code: %.4x, ", code);
          break;
        }
    }
  while (code != RVLChunkCode_END);
}

void
rvl_read_file_sig (RVL_t *self)
{
  rvlbyte_t sig[RVL_FILE_SIG_SIZE];
  rvl_read_data (self, sig, RVL_FILE_SIG_SIZE);

  for (rvlsize_t i = 0; i < RVL_FILE_SIG_SIZE; i++)
    {
      if (sig[i] != RVL_FILE_SIG[i])
        {
          fprintf (stderr, "%s\n", "Not an RVL file!");
          exit (EXIT_FAILURE);
        }
    }
}

void
rvl_read_INFO_chunk (RVL_t *self, rvlbyte_t *buffer)
{
  self->info->gridType = buffer[2];
  self->info->gridUnit = buffer[3];
  self->info->dataFormat = buffer[4];
  self->info->bitDepth = buffer[5];
  self->info->dataDimen = buffer[6];
  self->info->endian = buffer[7];

  memcpy (&self->info->resolution, &buffer[8], 12);
  memcpy (&self->info->voxelSize, &buffer[20], 12);
  memcpy (&self->info->position, &buffer[32], 12);
}

void
rvl_read_DATA_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size)
{
  rvl_data_alloc (self->data, self->info);

  char *const src = (char *)buffer;
  char *const dst = (char *)self->data->buffer;
  const rvlsize_t srcSize = size;
  const rvlsize_t dstCap = self->data->size;
  LZ4_decompress_safe (src, dst, srcSize, dstCap);
}

void
rvl_read_TEXT_chunk (RVL_t *self, rvlcbyte_t *buffer, rvlsize_t size)
{
  self->numText++;
  rvlsize_t keySize = 0;
  rvlsize_t valueSize = 0;

  for (rvlsize_t i = 0; i < size; i++)
    {
      if (buffer[i] == '\0')
        {
          keySize = i;
        }
    }

  valueSize = size - keySize;

  memcpy (self->text->key, buffer, keySize);

  self->text->value = (char *)malloc (valueSize);
  memcpy (self->text->value, buffer + keySize, valueSize);
}

