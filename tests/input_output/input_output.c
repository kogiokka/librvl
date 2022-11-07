#include <rvl.h>
#include <stdlib.h>
#include <string.h>

int
main ()
{
  RVL *rvl = rvl_create_writer ();

  rvl_set_volumetric_format (rvl, 2, 2, 2, RVL_PRIMITIVE_I8, RVL_ENDIAN_BIG);
  rvl_set_regular_grid (rvl, 1.0f, 1.0f, 1.0f);
  rvl_set_text (rvl, RVL_TAG_AUTHOR, "John Doe");

  int   size = rvl_eval_voxels_nbytes (rvl);
  void *data = (void *)malloc (size);
  memset (data, 'x', size);
  rvl_set_voxels (rvl, data);

  rvl_set_file (rvl, "io.rvl");
  rvl_write_rvl (rvl);

  rvl_set_io (rvl, stdout);
  rvl_write_rvl (rvl);

  rvl_set_io (rvl, stderr);
  rvl_set_io (rvl, stdout);
  rvl_set_io (rvl, stderr);
  rvl_write_rvl (rvl);

  rvl_set_file (rvl, "io_2.rvl");
  rvl_set_file (rvl, "io_3.rvl");
  rvl_write_rvl (rvl);

  free (data);
  rvl_destroy (&rvl);
}
