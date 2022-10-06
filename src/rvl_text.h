#ifndef RVL_TEXT_H
#define RVL_TEXT_H

#include "rvl_defs.h"

typedef struct RVLText
{
  char *key;
  char *value;
} RVLText_t;

RVLText_t *rvl_text_array_create (u32 num);
void rvl_text_array_destroy (RVLText_t **self);
void rvl_text_set (RVLText_t *self, char *key, char *value);

#endif
