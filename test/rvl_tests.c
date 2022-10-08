#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/rvl.h"

static RVLInfo *init_info ();

void
rvl_test_write_INFO ()
{
  RVLInfo *info = init_info ();

  RVL *rvl = rvl_create_writer ("test_INFO.rvl");
  rvl_set_INFO (rvl, &info);
  rvl_write (rvl);

  if (info != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_read_INFO ()
{
  RVL *rvl = rvl_create_reader ("test_INFO.rvl");

  rvl_read (rvl);

  RVLInfo *info = NULL;
  rvl_get_INFO (rvl, &info);

  RVLGridType gridType;
  RVLGridUnit unit;
  RVLDataFormat format;
  RVLBitDepth bitDepth;
  RVLDataDimen dimen;
  RVLEndian endian;
  int res[3];
  float vsize[3];
  float pos[3];

  rvl_info_get_grid (info, &gridType, &unit);
  rvl_info_get_data_form (info, &format, &bitDepth, &dimen);
  rvl_info_get_endian (info, &endian);
  rvl_info_get_resolution (info, &res[0], &res[1], &res[2]);
  rvl_info_get_voxel_size (info, &vsize[0], &vsize[1], &vsize[2]);
  rvl_info_get_position (info, &pos[0], &pos[1], &pos[2]);

  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data Form - format: %d, bits: %d, dimensions: %d\n",
           format, bitDepth, dimen);
  fprintf (stdout, "Endian - %d", endian);
  fprintf (stdout, "Resolution - x: %d, y: %d, z: %d\n", res[0], res[1],
           res[2]);
  fprintf (stdout, "Voxel Size - x: %.3f, y: %.3f, z: %.3f\n", vsize[0],
           vsize[1], vsize[2]);
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", pos[0], pos[1],
           pos[2]);

  if (unit != RVLGridUnit_NA)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_write_DATA ()
{
  RVLInfo *info = init_info ();

  RVLData *data = rvl_data_create ();

  RVLByte *buffer;

  rvl_data_alloc (data, info);
  RVLSize size = rvl_data_get_buffer (data, &buffer);
  memset (buffer, 'A', size);

  RVL *rvl = rvl_create_writer ("test_DATA.rvl");
  rvl_set_INFO (rvl, &info);
  rvl_set_DATA (rvl, &data);
  rvl_write (rvl);

  if (data != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_read_DATA ()
{
  RVL *rvl = rvl_create_reader ("test_DATA.rvl");

  rvl_read (rvl);

  RVLInfo *info;
  RVLData *data;

  rvl_get_INFO (rvl, &info);
  rvl_get_DATA (rvl, &data);

  unsigned char *buffer;
  RVLSize size = rvl_data_get_buffer (data, &buffer);

  fwrite (buffer, 1, size, stdout);

  rvl_destroy (&rvl);
}

void
rvl_test_write_TEXT ()
{
  RVLInfo *info = init_info ();
  int numText = 2;
  RVLText *textArr = rvl_text_array_create (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");

  RVL *rvl = rvl_create_writer ("test_TEXT.rvl");
  rvl_set_INFO (rvl, &info);
  rvl_set_TEXT (rvl, &textArr, numText);
  rvl_write (rvl);

  if (textArr != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_read_TEXT ()
{
  RVL *rvl = rvl_create_reader ("test_TEXT.rvl");
  rvl_read (rvl);
  RVLText *textArr;
  int numText;
  rvl_get_TEXT (rvl, &textArr, &numText);

  const char *key = NULL;
  const char *value = NULL;
  for (int i = 0; i < numText; i++)
    {
      rvl_text_get (textArr, i, &key, &value);
      fprintf (stdout, "%s: %s\n", key, value);
    }
  rvl_destroy (&rvl);
}

RVLInfo *
init_info ()
{
  RVLInfo *info = rvl_info_create ();
  rvl_info_set_grid (info, RVLGridType_Cartesian, RVLGridUnit_NA);
  rvl_info_set_data_form (info, RVLDataFormat_Unsigned, RVLBitDepth_8Bits,
                          RVLDataDimen_Scalar);
  rvl_info_set_endian (info, RVLEndian_Little);
  rvl_info_set_resolution (info, 20, 20, 20);
  rvl_info_set_voxel_size (info, 1.0f, 1.0f, 1.0f);
  return info;
}
