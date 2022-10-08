#include <lz4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detail/rvl_read_p.h"

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"
#include "detail/rvl_text_p.h"

void
rvl_read_chunk_header (RVL *self, RVLSize *size, RVLChunkCode_t *code)
{
  rvl_read_data (self, (RVLByte *)size, 4);
  rvl_read_data (self, (RVLByte *)code, 4);
}

void
rvl_read_chunk_payload (RVL *self, RVLByte *data, RVLSize size)
{
  rvl_read_data (self, data, size);
}

void
rvl_read_INFO_chunk (RVL *self, RVLByte *buffer, RVLSize size)
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
rvl_read_DATA_chunk (RVL *self, RVLByte *buffer, RVLSize size)
{
  self->data = rvl_data_create ();
  rvl_data_alloc (self->data, self->info);

  char *const src = (char *)buffer;
  char *const dst = (char *)self->data->buffer;
  const RVLSize srcSize = size;
  const RVLSize dstCap = self->data->size;
  LZ4_decompress_safe (src, dst, srcSize, dstCap);
}

void
rvl_read_TEXT_chunk (RVL *self, RVLByte *buffer, RVLSize size)
{
  RVLText *newArr = rvl_text_array_create (self->numText + 1);
  int numText = self->numText + 1;

  if (self->text != NULL)
    {
      memcpy (newArr, self->text, sizeof (RVLText) * self->numText);
      free (self->text);
    }

  RVLText *newText = &newArr[numText - 1];

  // Both the key and the value will be null-terminated.
  RVLSize nullPos = 0;
  for (RVLSize i = 0; i < size; i++)
    {
      if (buffer[i] == '\0')
        {
          nullPos = i;
        }
    }
  memcpy (newText->key, buffer, nullPos + 1);

  RVLSize valueSize = size - (nullPos + 1);
  newText->value = (char *)malloc (valueSize + 1);
  memcpy (newText->value, buffer + nullPos + 1, valueSize);
  newText->value[valueSize + 1] = '\0';

  self->text = newArr;
  self->numText = numText;
}

void
rvl_read_file_sig (RVL *self)
{
  RVLByte sig[RVL_FILE_SIG_SIZE];
  rvl_read_data (self, sig, RVL_FILE_SIG_SIZE);

  for (RVLSize i = 0; i < RVL_FILE_SIG_SIZE; i++)
    {
      if (sig[i] != RVL_FILE_SIG[i])
        {
          fprintf (stderr, "%s\n", "Not an RVL file!");
          exit (EXIT_FAILURE);
        }
    }
}

void
rvl_read_data (RVL *self, RVLByte *data, RVLSize size)
{
  if (self->readData == NULL)
    {
      fprintf (stderr,
               "[ERROR] The read data function is NULL. Might be due to wrong "
               "read/write instance of RVL_t.\n");
      exit (EXIT_FAILURE);
    }

  self->readData (self, data, size);
}

void
rvl_read_data_default (RVL *self, RVLByte *data, RVLSize size)
{
  if (self->io == NULL)
    {
      fprintf (stderr, "[ERROR] RVL_t is not initialized!");
      exit (EXIT_FAILURE);
    }

  const size_t count = fread (data, 1, size, self->io);

  if (count != size)
    {
      fprintf (stderr, "[ERROR] Read error occured!\n");
      exit (EXIT_FAILURE);
    }
}
