#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

void parent_path (char *dst, const char *path);
bool create_directory (const char *path);
bool create_directories (const char *path);

#endif
