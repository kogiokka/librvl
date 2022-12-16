/*
 * Author: Luis Kao
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

void parent_path (char *dst, const char *path);
bool create_directory (const char *path);
bool create_directories (const char *path);

#endif
