#ifndef RVL_READ_P_H
#define RVL_READ_P_H

#include "rvl_p.h"

void rvl_read_chunk_header (RVL_t *self, rvlsize_t *size,
                            RVLChunkCode_t *code);
void rvl_read_chunk_payload (RVL_t *self, rvlbyte_t *data, rvlsize_t size);

void rvl_read_INFO_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size);
void rvl_read_DATA_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size);
void rvl_read_TEXT_chunk (RVL_t *self, rvlbyte_t *buffer, rvlsize_t size);

void rvl_read_file_sig (RVL_t *self);
void rvl_read_data (RVL_t *self, rvlbyte_t *data, rvlsize_t size);

#endif
