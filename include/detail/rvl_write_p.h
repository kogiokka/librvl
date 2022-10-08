#ifndef RVL_WRITE_P_H
#define RVL_WRITE_P_H

#include "rvl_p.h"

void rvl_write_chunk_header (RVL *self, RVLChunkCode_t code, RVLSize size);
void rvl_write_chunk_payload (RVL *self, RVLConstByte *data, RVLSize size);
void rvl_write_chunk_end (RVL *self);

void rvl_write_INFO_chunk (RVL *self, const RVLInfo *info);
void rvl_write_DATA_chunk (RVL *self, const RVLData *data);
void rvl_write_TEXT_chunk (RVL *self, const RVLText *textArr, int numText);
void rvl_write_END_chunk (RVL *self);

void rvl_write_file_sig (RVL *self);
void rvl_write_data (RVL *self, RVLConstByte *data, RVLSize size);
void rvl_write_data_default (RVL *self, RVLConstByte *data, RVLSize size);

#endif
