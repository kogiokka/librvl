#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/rvl.h"

static void init_regular_grid (RVL *rvl);
static void init_rectilinear_grid (RVL *rvl);
static void print_data_buffer (int x, int y, int z,
                               const unsigned char *buffer, int primSize);

void
rvl_test_write_regular_grid ()
{
  RVL *rvl = rvl_create_writer ("test_regular.rvl");

  // VHDR/GRID chunk
  init_regular_grid (rvl);

  // DATA chunk
  int            size   = rvl_get_data_nbytes (rvl);
  unsigned char *buffer = (unsigned char *)malloc (size);
  memset (buffer, 'A', size);
  rvl_set_data_buffer (rvl, size, buffer);

  // TEXT chunk
  int      numText = 2;
  RVLText *textArr = rvl_text_create_array (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");
  rvl_set_text (rvl, &textArr, numText);

  // Write to file
  rvl_write_rvl (rvl);

  free (buffer);
  rvl_destroy (&rvl);
}

void
rvl_test_read_regular_grid ()
{
  RVL *rvl = rvl_create_reader ("test_regular.rvl");

  // Read from file
  rvl_read_rvl (rvl);

  // VHDR/GRID chunk
  RVLenum gridType = rvl_get_grid_type (rvl);
  RVLenum unit     = rvl_get_grid_unit (rvl);
  int     x, y, z;
  float   dx, dy, dz;
  float   px, py, pz;
  RVLenum primitive, endian;
  rvl_get_header (rvl, &x, &y, &z, &primitive, &endian);
  rvl_get_voxel_dims (rvl, &dx, &dy, &dz);
  rvl_get_grid_position (rvl, &px, &py, &pz);

  RVLenum compress = rvl_get_compression(rvl);

  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", primitive);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Compression method - %d\n", compress);
  fprintf (stdout, "Voxel Dim - x: %.3f, y: %.3f, z: %.3f\n", dx, dy, dz);
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", px, py, pz);
  fprintf (stdout, "%s\n", sep);

  if (unit != RVL_UNIT_NA)
    {
      exit (EXIT_FAILURE);
    }

  // DATA chunk
  unsigned char *buffer;
  rvl_get_data_buffer (rvl, &buffer);
  print_data_buffer (x, y, z, buffer, rvl_get_primitive_nbytes (rvl));

  // TEXT chunk
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

  rvl_destroy (&rvl);
}

void
rvl_test_write_rectilinear_grid ()
{
  RVL *rvl = rvl_create_writer ("test_rectilinear.rvl");

  // VHDR/GRID chunk
  init_rectilinear_grid (rvl);

  // DATA chunk
  int            size   = rvl_get_data_nbytes (rvl);
  unsigned char *buffer = (unsigned char *)malloc (size);
  memset (buffer, 'A', size);
  rvl_set_data_buffer (rvl, size, buffer);

  // TEXT chunk
  int      numText = 2;
  RVLText *textArr = rvl_text_create_array (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");
  rvl_set_text (rvl, &textArr, numText);

  // Write to file
  rvl_write_rvl (rvl);

  free (buffer);
  rvl_destroy (&rvl);
}

void
rvl_test_read_rectilinear_grid ()
{
  RVL *rvl = rvl_create_reader ("test_rectilinear.rvl");

  // Read from file
  rvl_read_rvl (rvl);

  // VHDR/GRID chunk
  RVLenum gridType = rvl_get_grid_type (rvl);
  RVLenum unit     = rvl_get_grid_unit (rvl);

  int          x, y, z;
  int          ndx, ndy, ndz;
  const float *dx, *dy, *dz;
  float        px, py, pz;
  RVLenum primitive, endian;
  rvl_get_header (rvl, &x, &y, &z, &primitive, &endian);
  rvl_get_voxel_dims_v (rvl, &ndx, &ndy, &ndz, &dx, &dy, &dz);
  rvl_get_grid_position (rvl, &px, &py, &pz);

  RVLenum compress = rvl_get_compression(rvl);

  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", primitive);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Compresson method - %d\n", compress);
  fprintf (stdout, "Voxel Dim -\n");

  if (ndx <= 0 || ndy <= 0 || ndx <= 0)
    {
      exit (EXIT_FAILURE);
    }

  for (int i = 0; i < ndx; i++)
    {
      fprintf (stdout, "    x%d: %.3f\n", i, dx[i]);
    }
  for (int i = 0; i < ndy; i++)
    {
      fprintf (stdout, "    y%d: %.3f\n", i, dy[i]);
    }
  for (int i = 0; i < ndz; i++)
    {
      fprintf (stdout, "    z%d: %.3f\n", i, dz[i]);
    }
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", px, py, pz);
  fprintf (stdout, "%s\n", sep);

  // DATA chunk
  unsigned char *buffer;
  rvl_get_data_buffer (rvl, &buffer);
  print_data_buffer (x, y, z, buffer, rvl_get_primitive_nbytes (rvl));

  // TEXT chunk
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

  rvl_destroy (&rvl);
}

void
rvl_test_partially_read ()
{
  RVL *rvl = rvl_create_reader ("test_regular.rvl");

  // Read VHDR, GRID and TEXT
  rvl_read_info (rvl);

  int         x, y, z;
  float       dx, dy, dz;
  float       px, py, pz;
  RVLText    *textArr;
  int         numText;
  const char *key   = NULL;
  const char *value = NULL;

  RVLenum gridType = rvl_get_grid_type (rvl);
  RVLenum unit     = rvl_get_grid_unit (rvl);
  RVLenum primitive, endian;

  rvl_get_header (rvl, &x, &y, &z, &primitive, &endian);
  rvl_get_voxel_dims (rvl, &dx, &dy, &dz);
  rvl_get_grid_position (rvl, &px, &py, &pz);
  rvl_get_text (rvl, &textArr, &numText);

  RVLenum compress = rvl_get_compression(rvl);

  // Print RVL information
  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", primitive);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Compression - %d\n", compress);
  fprintf (stdout, "Voxel Dim - x: %.3f, y: %.3f, z: %.3f\n", dx, dy, dz);
  for (int i = 0; i < numText; i++)
    {
      rvl_text_get (textArr, i, &key, &value);
      fprintf (stdout, "%s: %s\n", key, value);
    }

  // Read DATA
  unsigned char *buffer = (unsigned char *)malloc (rvl_get_data_nbytes (rvl));
  rvl_read_data_buffer (rvl, &buffer);
  print_data_buffer (x, y, z, buffer, rvl_get_primitive_nbytes (rvl));
  free (buffer);

  rvl_destroy (&rvl);
}

void
rvl_test_uninitialized_rvl ()
{
  RVL *rvl = NULL;
  rvl_destroy (&rvl);
}

void
init_regular_grid (RVL *rvl)
{
  rvl_set_header(rvl, 2, 2, 2, RVL_PRIMITIVE_VEC2U8, RVL_ENDIAN_LITTLE);

  rvl_set_grid_type (rvl, RVL_GRID_REGULAR);
  rvl_set_grid_unit (rvl, RVL_UNIT_NA);
  rvl_set_grid_position (rvl, 3.0f, 2.0f, 1.0f);
  rvl_set_voxel_dims (rvl, 0.1f, 0.2f, 0.3f);
}

void
init_rectilinear_grid (RVL *rvl)
{
  rvl_set_header(rvl, 6, 6, 3, RVL_PRIMITIVE_F8, RVL_ENDIAN_LITTLE);

  rvl_set_grid_type (rvl, RVL_GRID_RECTILINEAR);
  rvl_set_grid_unit (rvl, RVL_UNIT_NA);
  rvl_set_grid_position (rvl, 1.0f, 2.0f, 3.0f);

  float dx[6];
  float dy[6];
  float dz[3];

  for (int i = 0; i < 6; i++)
    {
      dx[i] = i * 2.0f;
      dy[i] = i * 0.4f;
      if (i < 3)
        {
          dz[i] = i * 3.0f;
        }
    }

  rvl_set_voxel_dims_v (rvl, 6, 6, 3, dx, dy, dz);
}

void
print_data_buffer (int x, int y, int z, const unsigned char *buffer,
                   int primSize)
{
  fprintf (stdout, "Prim size: %d\n", primSize);
  for (int k = 0; k < z; k++)
    {
      for (int j = 0; j < y; j++)
        {
          for (int i = 0; i < x - 1; i++)
            {
              fwrite (&buffer[i + j * x + k * x * y], 1, primSize, stdout);
              fprintf (stdout, "|");
            }
          fwrite (&buffer[x - 1 + j * x + k * x * y], 1, primSize, stdout);
          fprintf (stdout, "\n");
        }
      fprintf (stdout, "\n");
    }
}
