#include <rvl.h>
#include <stdlib.h>
#include <string.h>

static int     RES[3] = { 2, 2, 2 };
static RVLenum PRM    = RVL_PRIMITIVE_F32;
static RVLenum ENDIAN = RVL_ENDIAN_LITTLE;

int
main ()
{
  RVL *rvl = rvl_create_writer ();
  rvl_set_volumetric_format (rvl, RES[0], RES[1], RES[2], PRM, ENDIAN);
  rvl_set_regular_grid (rvl, 1.0f, 1.0f, 1.0f);

  float *data = (float *)malloc (RES[0] * RES[1] * RES[2] * rvl_sizeof (PRM));
  rvl_set_voxels (rvl, data);

  rvl_set_file (rvl, "read_info.rvl");
  rvl_write_rvl (rvl);

  free (data);
  rvl_destroy (&rvl);

  rvl = rvl_create_reader ();
  rvl_set_file (rvl, "read_info.rvl");

  data = (float *)malloc (RES[0] * RES[1] * RES[2] * rvl_sizeof (PRM));

  for (int i = 0; i < 10; i++)
    {
      rvl_read_voxels_to (rvl, data);
    }

  free (data);

  rvl_destroy (&rvl);
}
