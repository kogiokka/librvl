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
  RVL *rvl;

  rvl = rvl_create_writer ("volumetric_data.rvl");
  write_rvl (rvl);
  rvl_destroy (&rvl);

  rvl = rvl_create_reader ("volumetric_data.rvl");
  read_rvl (rvl);
  rvl_destroy (&rvl);

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

  rvl_set_text (rvl, RVL_TAG_TITLE, "Sphere");
  rvl_set_text (rvl, RVL_TAG_DESCRIPTION,
                "A volumetric model of an UV sphere.");
  rvl_set_text (rvl, RVL_TAG_AUTHOR, "John Doe");
  rvl_set_text (rvl, RVL_TAG_COPYRIGHT,
                "Copyright (c) 2022 by John Doe, All Rights Reserved.");
  rvl_set_text (rvl, RVL_TAG_LICENSE, "CC BY-SA 4.0");
  rvl_set_text (rvl, RVL_TAG_CREATION_TIME, "2022-10-26T05:13:44+08:00");

  // Write to file
  rvl_write_rvl (rvl);

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

  const char *title, *descr, *author, *copyright, *license, *source, *ctime;
  rvl_get_text (rvl, RVL_TAG_TITLE, &title);
  rvl_get_text (rvl, RVL_TAG_DESCRIPTION, &descr);
  rvl_get_text (rvl, RVL_TAG_AUTHOR, &author);
  rvl_get_text (rvl, RVL_TAG_COPYRIGHT, &copyright);
  rvl_get_text (rvl, RVL_TAG_LICENSE, &license);
  rvl_get_text (rvl, RVL_TAG_SOURCE, &source);
  rvl_get_text (rvl, RVL_TAG_CREATION_TIME, &ctime);

  fprintf (stdout, "Title: %s\n", title);
  fprintf (stdout, "Description: %s\n", descr);
  fprintf (stdout, "Author: %s\n", author);
  fprintf (stdout, "Copyright: %s\n", copyright);
  fprintf (stdout, "License: %s\n", license);
  fprintf (stdout, "Source: %s\n", source);
  fprintf (stdout, "Creation time: %s\n", ctime);
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
