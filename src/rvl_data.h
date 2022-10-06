#ifndef RVL_DATA_H
#define RVL_DATA_H

#include "rvl_defs.h"
#include "rvl_info.h"

typedef struct RVLData
{
  u8 *buffer;
  u32 size;
} RVLData_t;

RVLData_t *rvl_data_create (RVLInfo_t *info);
void rvl_data_destroy (RVLData_t **self);

#endif
