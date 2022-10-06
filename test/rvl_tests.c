#include "../src/rvl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static RVLInfo_t *init_info ();

void
rvl_test_info ()
{
  RVLInfo_t *info = init_info ();

  RVL_t *rvl = rvl_create ("test_INFO.rvl", RVLIoState_Write);
  rvl_set_INFO (rvl, &info);
  rvl_write (rvl);

  rvl_info_destroy (&rvl->info);
  if (info != NULL || rvl->info != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_data ()
{
  RVLInfo_t *info = init_info ();

  RVLData_t *data = rvl_data_create (info);
  memset (data->buffer, 'A', data->size);

  RVL_t *rvl = rvl_create ("test_DATA.rvl", RVLIoState_Write);
  rvl_set_INFO (rvl, &info);
  rvl_set_DATA (rvl, &data);
  rvl_write (rvl);

  rvl_data_destroy (&rvl->data);
  if (data != NULL || rvl->data != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

void
rvl_test_text ()
{
  RVLInfo_t *info = init_info ();

  int numText = 2;
  RVLText_t *text = rvl_text_array_create (numText);
  rvl_text_set (&text[0], "Title", "librvl");
  rvl_text_set (&text[1], "Description",
                "The Regular VoLumetric format reference library");

  RVL_t *rvl = rvl_create ("test_TEXT.rvl", RVLIoState_Write);
  rvl_set_INFO (rvl, &info);
  rvl_set_TEXT (rvl, &text, numText);
  rvl_write (rvl);

  rvl_text_array_destroy (&rvl->text);
  if (text != NULL || rvl->text != NULL)
    {
      exit (EXIT_FAILURE);
    }

  rvl_destroy (&rvl);
}

RVLInfo_t *
init_info ()
{
  RVLInfo_t *info = rvl_info_create ();
  rvl_info_set_grid (info, RVLGridType_Cartesian, RVLGridUnit_NA);
  rvl_info_set_data_form (info, RVLDataFormat_Unsigned, RVLBitDepth_8Bits,
                          RVLDataDimen_Scalar);
  rvl_info_set_endian (info, RVLEndian_Little);
  rvl_info_set_resolution (info, 20, 20, 20);
  rvl_info_set_voxel_size (info, 1.0f, 1.0f, 1.0f);
  return info;
}
