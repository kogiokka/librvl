#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "detail/rvl_log_p.h"

#define RVL_LOG_LEVEL_LAST RVL_LOG_LEVEL_FATAL

typedef void (*RVLLogFn) (RVLLog *self);

struct RVLLog
{
  RVLenum level;

  char        timestamp[80];
  const char *funcName;
  const char *format;
  va_list     args;

  RVLLogFn writeFn;
};

static void rvl_log_fwrite_default (RVLLog *self);
static void rvl_log_create_timestamp (RVLLog *self);

static RVLLog  logger       = { .writeFn = rvl_log_fwrite_default };
static RVLenum LOGGIN_LEVEL = RVL_LOG_LEVEL_TRACE;

static const char *levelStrings[RVL_LOG_LEVEL_LAST + 1] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL",
};

static int levelColors[RVL_LOG_LEVEL_LAST + 1] = { 94, 34, 93, 33, 31, 41 };
static int levelAttrs[RVL_LOG_LEVEL_LAST + 1]  = { 2, 1, 1, 1, 1, 1 };

void
rvl_log (RVLenum level, const char *funcName, const char *fmt, ...)
{
  if (level < LOGGIN_LEVEL)
    {
      return;
    }

  logger.level    = level;
  logger.funcName = funcName;
  logger.format   = fmt;

  va_start (logger.args, fmt);
  logger.writeFn (&logger);
  va_end (logger.args);
}

void
rvl_log_set_level (RVLenum level)
{
  LOGGIN_LEVEL = level;
}

void
rvl_log_fwrite_default (RVLLog *self)
{
  rvl_log_create_timestamp (self);

  // Calculate the required lengths
  size_t hdrLen = snprintf (
      NULL, 0, "%s \033[%d;%dm%s\033[0m \033[90m%s:\033[0m ", self->timestamp,
      levelColors[self->level], levelAttrs[self->level],
      levelStrings[self->level], self->funcName);

  va_list args;
  va_copy (args, self->args);
  size_t msgLen = vsnprintf (NULL, 0, self->format, args);
  va_end (args);

  // Write to char arrays
  char hdr[hdrLen + 1];
  char msg[msgLen + 1];
  snprintf (hdr, hdrLen + 1, "%s \033[%d;%dm%s\033[0m \033[90m%s\033[0m",
            self->timestamp, levelColors[self->level], levelAttrs[self->level],
            levelStrings[self->level], self->funcName);
  vsnprintf (msg, msgLen + 1, self->format, self->args);

  // Print the char arrays at once
  fprintf (stdout, "%s: %s\n", hdr, msg);
  fflush (stdout);
}

void
rvl_log_create_timestamp (RVLLog *self)
{
  char *buf = self->timestamp;

  time_t now;
  time (&now);
  buf[strftime (buf, sizeof (self->timestamp), "%FT%TZ", gmtime (&now))]
      = '\0';
}
