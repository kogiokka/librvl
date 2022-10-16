#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/rvl.h"

static void init_regular_grid (RVL *rvl);
static void init_rectilinear_grid (RVL *rvl);

void
rvl_test_write_regular_grid ()
{
  RVL *rvl = rvl_create_writer ("test_regular.rvl");

  // VHDR/GRID chunk
  init_regular_grid (rvl);

  // DATA chunk
  RVLByte *buffer;
  RVLSize  size;
  rvl_alloc_data_buffer (rvl, &buffer, &size);
  memset (buffer, 'A', size);
  rvl_set_data_buffer (rvl, buffer, size);
  rvl_write_rvl (rvl);
  rvl_dealloc_data_buffer (rvl, &buffer);

  // TEXT chunk
  int      numText = 2;
  RVLText *textArr = rvl_text_create_array (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");
  rvl_set_text (rvl, &textArr, numText);

  // Write to file
  rvl_write_rvl (rvl);

  rvl_destroy (&rvl);
}

void
rvl_test_read_regular_grid ()
{
  RVL *rvl = rvl_create_reader ("test_regular.rvl");

  // Read from file
  rvl_read_rvl (rvl);

  // VHDR/GRID chunk
  RVLGridType  gridType = rvl_get_grid_type (rvl);
  RVLGridUnit  unit     = rvl_get_grid_unit (rvl);
  RVLPrimitive format   = rvl_get_primitive (rvl);
  RVLEndian    endian   = rvl_get_endian (rvl);
  int          x, y, z;
  float        dx, dy, dz;
  float        px, py, pz;
  rvl_get_resolution (rvl, &x, &y, &z);
  rvl_get_voxel_dimensions (rvl, &dx, &dy, &dz);
  rvl_get_position (rvl, &px, &py, &pz);

  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", format);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Voxel Dim - x: %.3f, y: %.3f, z: %.3f\n", dx, dy, dz);
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", px, py, pz);
  fprintf (stdout, "%s\n", sep);

  if (unit != RVLGridUnit_NA)
    {
      exit (EXIT_FAILURE);
    }

  // DATA chunk
  RVLByte *buffer;
  RVLSize  size;
  rvl_get_data_buffer (rvl, &buffer, &size);
  fwrite (buffer, 1, size, stdout);

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
  RVLByte *buffer;
  RVLSize  size;
  rvl_alloc_data_buffer (rvl, &buffer, &size);
  memset (buffer, 'A', size);
  rvl_set_data_buffer (rvl, buffer, size);
  rvl_write_rvl (rvl);
  rvl_dealloc_data_buffer (rvl, &buffer);

  // TEXT chunk
  int      numText = 2;
  RVLText *textArr = rvl_text_create_array (numText);
  rvl_text_set (textArr, 0, "Title", "librvl");
  rvl_text_set (textArr, 1, "Description",
                "The Regular VoLumetric format reference library");
  rvl_set_text (rvl, &textArr, numText);

  // Write to file
  rvl_write_rvl (rvl);

  rvl_destroy (&rvl);
}

void
rvl_test_read_rectilinear_grid ()
{
  RVL *rvl = rvl_create_reader ("test_rectilinear.rvl");

  // Read from file
  rvl_read_rvl (rvl);

  // VHDR/GRID chunk
  RVLGridType  gridType = rvl_get_grid_type (rvl);
  RVLGridUnit  unit     = rvl_get_grid_unit (rvl);
  RVLPrimitive format   = rvl_get_primitive (rvl);
  RVLEndian    endian   = rvl_get_endian (rvl);
  int          x, y, z;
  const float *vsize;
  float        px, py, pz;
  rvl_get_resolution (rvl, &x, &y, &z);
  rvl_get_voxel_dimensions_v (rvl, &vsize);
  rvl_get_position (rvl, &px, &py, &pz);

  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", format);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Voxel Dim -\n");
  for (int i = 0; i < x; i++)
    {
      fprintf (stdout, "    x%d: %.3f\n", i, vsize[i]);
    }
  for (int i = 0; i < y; i++)
    {
      fprintf (stdout, "    y%d: %.3f\n", i, vsize[i + x]);
    }
  for (int i = 0; i < z; i++)
    {
      fprintf (stdout, "    z%d: %.3f\n", i, vsize[i + x + y]);
    }
  fprintf (stdout, "Position - x: %.3f, y: %.3f, z: %.3f\n", px, py, pz);
  fprintf (stdout, "%s\n", sep);

  // DATA chunk
  RVLByte *buffer;
  RVLSize  size;
  rvl_get_data_buffer (rvl, &buffer, &size);
  fwrite (buffer, 1, size, stdout);

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

  int          x, y, z;
  const float *vsize;
  float        px, py, pz;
  RVLText     *textArr;
  int          numText;
  const char  *key   = NULL;
  const char  *value = NULL;

  RVLGridType  gridType = rvl_get_grid_type (rvl);
  RVLGridUnit  unit     = rvl_get_grid_unit (rvl);
  RVLPrimitive format   = rvl_get_primitive (rvl);
  RVLEndian    endian   = rvl_get_endian (rvl);
  rvl_get_resolution (rvl, &x, &y, &z);
  rvl_get_voxel_dimensions_v (rvl, &vsize);
  rvl_get_position (rvl, &px, &py, &pz);
  rvl_get_text (rvl, &textArr, &numText);

  // Print RVL information
  char sep[81];
  memset (sep, '-', 80);
  fprintf (stdout, "%s\n", sep);
  fprintf (stdout, "Width: %d, Length: %d, Height: %d\n", x, y, z);
  fprintf (stdout, "Grid - type: %d, unit: %d\n", gridType, unit);
  fprintf (stdout, "Data format: 0x%.4x\n", format);
  fprintf (stdout, "Endian - %d\n", endian);
  fprintf (stdout, "Voxel Dim - x: %.3f, y: %.3f, z: %.3f\n", vsize[0], vsize[1], vsize[2]);
  for (int i = 0; i < numText; i++)
    {
      rvl_text_get (textArr, i, &key, &value);
      fprintf (stdout, "%s: %s\n", key, value);
    }

  // Read DATA
  RVLByte *buffer = NULL;
  RVLSize  size;
  rvl_alloc_data_buffer (rvl, &buffer, &size);
  rvl_read_data_buffer (rvl, &buffer);
  fwrite (buffer, 1, size, stdout);
  rvl_dealloc_data_buffer (rvl, &buffer);

  rvl_destroy (&rvl);
}

void
init_regular_grid (RVL *rvl)
{
  rvl_set_grid_type (rvl, RVLGridType_Cartesian);
  rvl_set_grid_unit (rvl, RVLGridUnit_NA);

  rvl_set_primitive (rvl, RVLPrimitive_vec2u8);
  rvl_set_endian (rvl, RVLEndian_Little);
  rvl_set_resolution (rvl, 2, 2, 2);
  rvl_set_position (rvl, 3.0f, 2.0f, 1.0f);

  rvl_set_voxel_dimensions (rvl, 0.1f, 0.2f, 0.3f);
}

void
init_rectilinear_grid (RVL *rvl)
{
  rvl_set_grid_type (rvl, RVLGridType_Rectilinear);
  rvl_set_grid_unit (rvl, RVLGridUnit_NA);

  rvl_set_primitive (rvl, RVLPrimitive_u8);
  rvl_set_endian (rvl, RVLEndian_Little);
  rvl_set_resolution (rvl, 6, 6, 3);
  rvl_set_position (rvl, 1.0f, 2.0f, 3.0f);

  int   numDim = 6 + 6 + 3;
  float dims[numDim];

  for (int i = 0; i < numDim; i++)
    {
      dims[i] = i * 2.0f;
    }

  rvl_set_voxel_dimensions_v (rvl, numDim, dims);
}
