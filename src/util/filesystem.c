#include "util/filesystem.h"
#include "util/logger.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif (defined(_WIN32) || defined(__WIN32__))
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define NPOS          -1
#define MAX_DIR_DEPTH 80

static int find_last_of (const char *str, char c, int pos);
static int find_last_not_of (const char *str, char c, int pos);

bool
create_directory (const char *path)
{
  log_trace ("Creating directory \"%s\"", path);

#if defined(__unix__) || defined(__unix) || defined(__linux__)
  int status = mkdir (path, S_IRWXU | S_IRWXG | S_IRWXO);
#elif (defined(_WIN32) || defined(__WIN32__))
  int status = _mkdir (path);
#endif

  if (status != 0 && errno != EEXIST)
    {
      log_error ("%s", strerror (errno));
      return false;
    }

  return true;
}

bool
create_directories (const char *path)
{
  int   len = strlen (path);
  char *buf = (char *)malloc (len + 1);
  strcpy (buf, path);

  int   depth = 0;
  char *stack[MAX_DIR_DEPTH];
  stack[0] = buf;

  for (;;)
    {
      if (depth >= MAX_DIR_DEPTH)
        {
          log_error ("Max directory depth reached!");
          break;
        }

      char *tmp = (char *)malloc (len + 1);
      parent_path (tmp, buf);

      if (strcmp (tmp, "") == 0)
        {
          break;
        }

      depth++;
      stack[depth] = tmp;
      buf          = tmp;
    }

  for (int i = depth; i >= 0; i--)
    {
      log_trace ("Creating directory \"%s\"", stack[i]);
#if defined(__unix__) || defined(__unix) || defined(__linux__)
      int status = mkdir (stack[i], S_IRWXU | S_IRWXG | S_IRWXO);
#elif (defined(_WIN32) || defined(__WIN32__))
      int status = _mkdir (stack[i]);
#endif

      free (stack[i]);

      if (status != 0 && errno != EEXIST)
        {
          log_error ("%s", strerror (errno));
          for (int j = i - 1; j >= 0; j--)
            {
              free (stack[j]);
            }
          return false;
        }
    }

  return true;
}

void
parent_path (char *dst, const char *path)
{
  int len     = strlen (path);
  int lastPos = len - 1;

  if (path[lastPos] == '/' || path[lastPos] == '/')
    {
      lastPos -= 1;
    }

  int pos = find_last_not_of (path, '/', find_last_of (path, '/', lastPos));

  if (pos == NPOS)
    {
      pos = find_last_not_of (path, '\\', find_last_of (path, '\\', lastPos));
    }

  dst[pos + 1] = '\0';
  memcpy (dst, path, pos + 1);
}

int
find_last_of (const char *str, char c, int pos)
{
  for (int i = pos; i >= 0; i--)
    {
      if (str[i] == c)
        {
          break;
        }
      pos--;
    }

  return pos;
}

int
find_last_not_of (const char *str, char c, int pos)
{
  for (int i = pos; i >= 0; i--)
    {
      if (str[i] != c)
        {
          break;
        }
      pos--;
    }

  return pos;
}
