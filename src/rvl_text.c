#include <stdlib.h>

#include "detail/rvl_p.h"
#include "detail/rvl_text_p.h"

RVLText_t *
rvl_text_array_create (int num)
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
rvl_text_set (RVLText_t *textArr, int index, char *key, char *value)
{
  textArr[index].key = key;
  textArr[index].value = value;
}
