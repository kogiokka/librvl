#ifndef RVL_WRITE_P_H
#define RVL_WRITE_P_H

#include "rvl_p.h"

void rvl_write_chunk_header (RVL_t *self, RVLChunkCode_t code, rvlsize_t size);
void rvl_write_chunk_payload (RVL_t *self, rvlcbyte_t *data, rvlsize_t size);
void rvl_write_chunk_end (RVL_t *self);

void rvl_write_INFO_chunk (RVL_t *self, const RVLInfo_t *info);
void rvl_write_DATA_chunk (RVL_t *self, const RVLData_t *data);
void rvl_write_TEXT_chunk (RVL_t *self, const RVLText_t *textArr, int numText);
void rvl_write_END_chunk (RVL_t *self);

void rvl_write_file_sig (RVL_t *self);
void rvl_write_data (RVL_t *self, rvlcbyte_t *data, rvlsize_t size);

#endif
