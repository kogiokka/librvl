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
static void rvl_read_INFO_chunk (RVL_t *self, rvlbyte_t *buffer,
                                 rvlsize_t size);
static void rvl_read_DATA_chunk (RVL_t *self, rvlbyte_t *buffer,
                                 rvlsize_t size);
static void rvl_read_TEXT_chunk (RVL_t *self, rvlbyte_t *buffer,
                                 rvlsize_t size);

void
rvl_read_data (RVL_t *self, rvlbyte_t *data, rvlsize_t size)
{
  const size_t count = fread (data, 1, size, self->io);

  if (count != size)
    {
      fprintf (stderr, "[ERROR] Read error occured!\n");
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
rvl_read_INFO_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size)
{
  self->info = rvl_info_create ();
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
  self->data = rvl_data_create ();
  rvl_data_alloc (self->data, self->info);

  rvl_read_data (self, buffer, size);

  char *const src = (char *)buffer;
  char *const dst = (char *)self->data->buffer;
  const rvlsize_t srcSize = size;
  const rvlsize_t dstCap = self->data->size;
  LZ4_decompress_safe (src, dst, srcSize, dstCap);
}

void
rvl_read_TEXT_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size)
{
  RVLText_t *newArr = rvl_text_array_create (self->numText + 1);
  for (int i = 0; i < self->numText; i++)
    {
      strcpy (newArr[i].key, self->text[i].key);
      newArr[i].value = self->text[i].value;
      self->text[i].value = NULL;
    }

  const int index = self->numText + 1;
  RVLText_t *newText = &newArr[index];

  rvlsize_t keySize = 0;
  rvlsize_t valueSize = 0;

  for (rvlsize_t i = 0; i < size; i++)
    {
      if (buffer[i] == '\0')
        {
          keySize = i + 1;
        }
    }

  valueSize = size - (keySize + 1);

  memcpy (newText->key, buffer, keySize + 1);

  newText->value = (char *)malloc (valueSize + 1);
  memcpy (newText->value, buffer + keySize + 1, valueSize);
  newText->value[valueSize + 1] = '\0';

  if (self->text != NULL)
    {
      for (int i = 0; i < self->numText; i++)
        {
          free (self->text[i].value);
        }
      free (self->text);
    }

  self->text = newArr;
  self->numText += 1;
}
