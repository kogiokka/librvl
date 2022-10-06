#include <stdlib.h>

#include "rvl_text.h"

RVLText_t *
rvl_text_array_create (u32 num)
{
  RVLText_t *textarr = (RVLText_t *)malloc (sizeof (RVLText_t) * num);
  return textarr;
}

void
rvl_text_array_destroy (RVLText_t **self)
{
  if (*self == NULL)
    {
      return;
    }

  free (*self);
  *self = NULL;
}

void
rvl_text_set (RVLText_t *self, char *key, char *value)
{
  self->key = key;
  self->value = value;
}
