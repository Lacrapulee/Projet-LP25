#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stddef.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


void read_file(const char *filepath, size_t *size);
char **list_files(const char *path); 
const char *get_last_directory(const char *path);

#endif // FILE_HANDLER_H

