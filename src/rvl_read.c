#include <lz4.h>

#include "detail/rvl_p.h"

#include "detail/rvl_data_p.h"
#include "detail/rvl_info_p.h"
#include "detail/rvl_text_p.h"

void
rvl_read (RVL_t *self)
{
  // TODO
}

void
rvl_get_INFO (RVL_t *self, RVLInfo_t **info)
{
  *info = self->info;
}

void
rvl_get_DATA (RVL_t *self, RVLData_t **data)
{
  *data = self->data;
}

void
rvl_get_TEXT (RVL_t *self, RVLText_t **text, int *numText)
{
  *text = self->text;
  *numText = self->numText;
}
