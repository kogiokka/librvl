#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rvl.h>

static void write_rvl (RVL *rvl);
static void read_rvl (RVL *rvl);
static void print_data_buffer (int x, int y, int z,
                               const unsigned char *buffer);

int
main ()
{
  RVL *rvlw = rvl_create_writer ("volumetric_data.rvl");
  write_rvl (rvlw);
  rvl_destroy (&rvlw);

  RVL *rvlr = rvl_create_reader ("volumetric_data.rvl");
  read_rvl (rvlr);
  rvl_destroy (&rvlr);
  return 0;
}

void
write_rvl (RVL *rvl)
{
  int nx = 20;
  int ny = 10;
  int nz = 20;

  unsigned int size   = nx * ny * nz * sizeof (int);
  int         *buffer = (int *)malloc (size);

  for (int i = 0; i < nz; i++)
    {
      for (int j = 0; j < ny; j++)
        {
          for (int k = 0; k < nx; k++)
            {
              buffer[k + j * nx + i * nx * ny] = i;
            }
        }
    }

  rvl_set_regular_grid (rvl, 0.1f, 0.2f, 0.1f);
  rvl_set_volumetric_format (rvl, nx, ny, nz, RVL_PRIMITIVE_I32,
                             RVL_ENDIAN_LITTLE);
  rvl_set_data_buffer (rvl, size, (unsigned char *)buffer);

  int      numText = 2;
  RVLText *textArr = rvl_text_create_array (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");
  rvl_set_text (rvl, textArr, numText);

  // Write to file
  rvl_write_rvl (rvl);

  rvl_text_destroy_array (&textArr);
  free (buffer);
}

void
read_rvl (RVL *rvl)
{
  // Read from file
  rvl_read_rvl (rvl);

  RVLenum gridType = rvl_get_grid_type (rvl);
  RVLenum unit     = rvl_get_grid_unit (rvl);
  RVLenum primitive, endian;
  int     x, y, z;
  float   dx, dy, dz;
  float   px, py, pz;
  rvl_get_volumetric_format (rvl, &x, &y, &z, &primitive, &endian);
  rvl_get_voxel_dims (rvl, &dx, &dy, &dz);
  rvl_get_grid_position (rvl, &px, &py, &pz);

  char sep[81];
  memset (sep, '-', 80);
  sep[80] = '\0';
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", primitive);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Voxel Dim - x: %.3f, y: %.3f, z: %.3f\n", dx, dy, dz);
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", px, py, pz);
  fprintf (stdout, "%s\n", sep);

  unsigned char *buffer;
  rvl_get_data_buffer (rvl, &buffer);
  print_data_buffer (x, y, z, buffer);

  RVLText *textArr;
  int      numText;
  rvl_get_text (rvl, &textArr, &numText);

  const char *key   = NULL;
  const char *value = NULL;
  for (int i = 0; i < numText; i++)
    {
      rvl_text_get (textArr, i, &key, &value);
      fprintf (stdout, "%s: %s\n", key, value);
    }
}

void
print_data_buffer (int x, int y, int z, const unsigned char *buffer)
{
  const int *data = (int *)buffer;
  for (int k = 0; k < z; k++)
    {
      for (int j = 0; j < y; j++)
        {
          for (int i = 0; i < x - 1; i++)
            {
              fprintf (stdout, "%d|", data[i + j * x + k * x * y]);
            }
          fprintf (stdout, "%d", data[x - 1 + j * x + k * x * y]);
          fprintf (stdout, "\n");
        }
      fprintf (stdout, "\n");
    }
}
