#include <stdbool.h>
#include <stdlib.h>

#include <log.h>
#include <lzma.h>

#include "detail/rvl_compress_p.h"

static void     create_lzma_encoder (lzma_stream *self, uint32_t preset);
static void     destroy_lzma_encoder (lzma_stream *self);
static lzma_ret run_lzma_compression (lzma_stream *strm, const BYTE *src,
                                      u32 srcSize, BYTE *dst, u32 dstCap);

static bool init_lzma_decoder (lzma_stream *strm);
static u32  decompress_lzma (lzma_stream *strm, const BYTE *src, u32 srcSize,
                             BYTE **dst, u32 dstCap);

static void print_lzma_compression_error (lzma_ret ret);
static void print_lzma_decompression_error (lzma_ret ret);

void
rvl_compress_lzma (RVL *self, BYTE **out, u32 *size)
{
  lzma_stream strm;

  const BYTE *src     = self->data.wbuf;
  u32         srcSize = self->data.size;
  u32         dstCap  = self->data.size;

  log_trace ("[librvl compress][LZMA] Compressing data with %u bytes...",
             srcSize);

  lzma_ret ret;
  create_lzma_encoder (&strm, 6 | LZMA_PRESET_DEFAULT);
  ret = run_lzma_compression (&strm, src, srcSize, *out, dstCap);

  if (ret != LZMA_STREAM_END)
    {
      destroy_lzma_encoder (&strm);
      create_lzma_encoder (&strm, 6 | LZMA_PRESET_DEFAULT);

      dstCap = lzma_stream_buffer_bound (self->data.size);
      *out   = realloc (*out, dstCap);

      log_trace (
          "[librvl compress][LZMA] Reallocate output memory with %u bytes.",
          dstCap);

      ret = run_lzma_compression (&strm, src, srcSize, *out, dstCap);
    }

  if (ret != LZMA_STREAM_END)
    {
      print_lzma_compression_error (ret);
      exit (EXIT_FAILURE);
    }

  *size = strm.total_out;
  destroy_lzma_encoder (&strm);

  log_trace ("[librvl compress][LZMA] Compressed successfully. The output "
             "size is %u bytes.",
             strm.total_out);
}

void
rvl_decompress_lzma (RVL *self, const BYTE *in, u32 size)
{
  lzma_stream strm = LZMA_STREAM_INIT;
  init_lzma_decoder (&strm);
  u32 nbytes
      = decompress_lzma (&strm, in, size, &self->data.rbuf, self->data.size);

  if (nbytes != self->data.size)
    {
      log_fatal ("[librvl compress][LZMA] Decompression failed. The data size "
                 "should be %u. However, the decompressed data size was %u.",
                 self->data.size, nbytes);
      exit (EXIT_FAILURE);
    }
  lzma_end (&strm);
}

void
create_lzma_encoder (lzma_stream *self, uint32_t preset)
{
  *self = (lzma_stream)LZMA_STREAM_INIT;

  lzma_ret ret = lzma_easy_encoder (self, preset, LZMA_CHECK_CRC64);

  // Return successfully
  if (ret == LZMA_OK)
    return;

  print_lzma_compression_error (ret);
}

void
destroy_lzma_encoder (lzma_stream *self)
{
  lzma_end (self);
}

lzma_ret
run_lzma_compression (lzma_stream *strm, const BYTE *src, u32 srcSize,
                      BYTE *dst, u32 dstCap)
{
  strm->next_in   = src;
  strm->avail_in  = srcSize;
  strm->next_out  = dst;
  strm->avail_out = dstCap;

  return lzma_code (strm, LZMA_FINISH);
}

bool
init_lzma_decoder (lzma_stream *strm)
{
  lzma_ret ret = lzma_stream_decoder (strm, UINT32_MAX, LZMA_CONCATENATED);

  // Return successfully
  if (ret == LZMA_OK)
    return true;

  print_lzma_decompression_error (ret);
  return false;
}

u32
decompress_lzma (lzma_stream *strm, const BYTE *src, u32 srcSize, BYTE **dst,
                 u32 dstCap)
{
  strm->next_in  = src;
  strm->avail_in = srcSize;

  strm->next_out  = *dst;
  strm->avail_out = dstCap;

  lzma_ret ret;
  ret = lzma_code (strm, LZMA_RUN);
  ret = lzma_code (strm, LZMA_FINISH);

  if (ret == LZMA_STREAM_END)
    {
      log_trace ("[librvl compress][LZMA] Decompressed size: %u",
                 strm->total_out);
      return strm->total_out;
    }

  print_lzma_decompression_error (ret);
  return 0;
}

void
print_lzma_compression_error (lzma_ret ret)
{
  const char *msg;
  switch (ret)
    {
    case LZMA_MEM_ERROR:
      msg = "Memory allocation failed";
      break;
    case LZMA_DATA_ERROR:
      msg = "File size limits exceeded";
      break;
    case LZMA_OPTIONS_ERROR:
      msg = "Specified preset is not supported";
      break;
    case LZMA_UNSUPPORTED_CHECK:
      msg = "Specified integrity check is not supported";
      break;
    default:
      msg = "Unknown error, possibly a bug";
      break;
    }

  log_error (
      "[librvl compress][LZMA] Compression failed: %s (error code %u)\n", msg,
      ret);
}

void
print_lzma_decompression_error (lzma_ret ret)
{
  const char *msg;
  switch (ret)
    {
    case LZMA_MEM_ERROR:
      msg = "Memory allocation failed";
      break;
    case LZMA_FORMAT_ERROR:
      msg = "The input is not in the .xz format";
      break;
    case LZMA_OPTIONS_ERROR:
      msg = "Unsupported compression options";
      break;
    case LZMA_DATA_ERROR:
      msg = "Compressed file is corrupt";
      break;
    case LZMA_BUF_ERROR:
      msg = "Compressed file is truncated or "
            "otherwise corrupt";
      break;
    default:
      msg = "Unknown error, possibly a bug";
      break;
    }

  log_error (
      "[librvl compress][LZMA] Decompression failed: %s (error code %u)\n",
      msg, ret);
}
