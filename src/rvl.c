#include <lz4.h>
#include <lz4hc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rvl.h"

#define SIG_SIZE 12

static void rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code,
                                    u32 size);
static void rvl_write_chunk_payload (RVL_t *self, u8const *data, u32 size);
static void rvl_write_chunk_end (RVL_t *self);
static void rvl_write_data (RVL_t *self, u8const *data, u32 size);

static void rvl_write_file_sig (RVL_t *self);
static void rvl_write_INFO_chunk (RVL_t *self, const RVLInfo_t *info);
static void rvl_write_DATA_chunk (RVL_t *self, const RVLData_t *data);
static void rvl_write_TEXT_chunk (RVL_t *self, const RVLText_t *arrText,
                                  int numText);
static void rvl_write_END_chunk (RVL_t *self);

RVL_t *
rvl_create (const char *filename, RVLIoState_t ioState)
{
  RVL_t *self = (RVL_t *)malloc (sizeof (RVL_t));
  self->version[0] = RVL_VERSION_MAJOR;
  self->version[1] = RVL_VERSION_MINOR;
  self->ioState = ioState;

  switch (ioState)
    {
    case RVLIoState_Read:
      self->io = fopen (filename, "rb");
      break;
    case RVLIoState_Write:
      self->io = fopen (filename, "wb");
      break;
    }

  return self;
}

void
rvl_destroy (RVL_t **self)
{
  RVL_t *ptr = *self;
  rvl_info_destroy (&ptr->info);
  rvl_text_array_destroy (&ptr->text);
  rvl_data_destroy (&ptr->data);

  fclose (ptr->io);
  free (ptr);

  *self = NULL;
}

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
rvl_read (RVL_t *self)
{
  // TODO
}

void
rvl_write_file_sig (RVL_t *self)
{
  // .RVL FORMAT\0
  u8 signature[SIG_SIZE] = {
    131, 82, 86, 76, 32, 70, 79, 82, 77, 65, 84, 0,
  };

  rvl_write_data (self, signature, SIG_SIZE);
}

void
rvl_write_INFO_chunk (RVL_t *self, const RVLInfo_t *info)
{
  u32 byteSize = 44;
  u8 *buf = calloc (1, byteSize);

  memcpy (&buf[0], self->version, 2);
  buf[2] = info->gridType;
  buf[3] = info->gridUnit;
  buf[4] = info->dataDimen;
  buf[5] = info->bitDepth;
  buf[6] = info->endian;
  memcpy (&buf[7], info->resolution, 12);
  memcpy (&buf[19], info->voxelSize, 12);
  memcpy (&buf[31], info->coordinates, 12);

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
  rvl_write_chunk_payload (self, (u8const *)compressedBuf, compressedSize);
  rvl_write_chunk_end (self);
}

void
rvl_write_TEXT_chunk (RVL_t *self, const RVLText_t *arrText, int numText)
{
  for (int i = 0; i < numText; i++)
    {
      const RVLText_t *const text = &arrText[i];

      char keyword[80];
      u32 keySize = strlen (text->key);
      u32 valueSize = strlen (text->value);

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
      rvl_write_chunk_payload (self, (u8const *)keyword, keySize + 1);

      if (valueSize != 0)
        {
          rvl_write_chunk_payload (self, (u8const *)text->value, valueSize);
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
rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code, u32 size)
{
  u8 buf[8];

  // Chunk Size(bytes)
  memcpy (buf, &size, 4);
  // Chunk Code
  memcpy (&buf[4], &code, 4);

  rvl_write_data (self, buf, 8);
}

void
rvl_write_chunk_payload (RVL_t *self, u8const *data, u32 size)
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
rvl_write_data (RVL_t *self, u8const *data, u32 size)
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
