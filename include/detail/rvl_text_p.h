#ifndef RVL_TEXT_P_H
#define RVL_TEXT_P_H

#ifndef RVL_H_INTERNAL
#error Never include this file directly. Use <rvl.h> instead.
#endif

struct RVLText
{
  char    key[80];
  char   *value;
  RVLenum field;

  struct RVLText *next;
};

RVLText *rvl_text_create ();
void     rvl_text_destroy (RVLText **self);
void     rvl_text_set_field (RVLText *self, RVLenum field, const char *value);

#endif
