#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

  free ((*self)->value);
  free (*self);
  *self = NULL;
}

void
rvl_text_set (RVLText_t *textArr, int index, char *key, char *value)
{
  RVLText_t *text = &textArr[index];

  size_t keySize = strlen (key);
  size_t valueSize = strlen (value);

  if (keySize >= 80)
    {
      fprintf (stderr,
               "[WARNING] Key length exceeds the maximum limit and will "
               "be truncated:\n \"%s\"\n",
               key);
      keySize = 79;
    }
  if (valueSize > sizeof (rvlsize_t) - keySize)
    {
      fprintf (stderr,
               "[WARNING] Value length exceeds the maximum limit and will "
               "be truncated:\n \"%s\"\n",
               value);
      valueSize = sizeof (rvlsize_t) - keySize;
    }

  text->value = (char *)malloc (valueSize);

  memcpy (text->key, key, keySize);
  text->key[keySize + 1] = '\0';
  memcpy (text->value, value, valueSize);

  text->keySize = keySize;
  text->valueSize = valueSize;
}
