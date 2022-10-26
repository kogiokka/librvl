#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <log.h>

#include "detail/rvl_p.h"
#include "detail/rvl_text_p.h"

RVLText *
rvl_text_create ()
{
  RVLText *self = (RVLText *)calloc (1, sizeof (RVLText));
  return self;
}

void
rvl_text_destroy (RVLText **self)
{
  if (self == NULL)
    return;

  free (*self);
  *self = NULL;
}

void
rvl_text_set_field (RVLText *self, RVLenum field, const char *value)
{
  switch (field)
    {
    case RVL_TEXT_FIELD_TITLE:
      strcpy (self->key, "Title");
      break;
    case RVL_TEXT_FIELD_AUTHOR:
      strcpy (self->key, "Author");
      break;
    case RVL_TEXT_FIELD_LICENSE:
      strcpy (self->key, "License");
      break;
    case RVL_TEXT_FIELD_DESCRIPTION:
      strcpy (self->key, "Description");
      break;
    default:
      log_error ("[librvl set] Unknown text field: %.4x", field);
      return;
    }

  u32 size    = strlen (value) + 1; // including '\0'
  self->value = (char *)malloc (size);
  memcpy (self->value, value, size);
}
