#ifndef RVL_TEXT_P_H
#define RVL_TEXT_P_H

#ifndef RVL_H_INTERNAL
#error Never include this file directly. Use <rvl.h> instead.
#endif

struct RVLText
{
  char  key[80];
  char *value;
};

#endif
