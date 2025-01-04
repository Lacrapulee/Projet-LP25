#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stddef.h>

/*void read_file(const char *filepath, size_t *size);
void write_file(const char *filepath, const void *data, size_t size);

*/
char **list_files(const char *path); 
char** list_folder(const char *path);
const char *get_last_directory(const char *path);

#endif // FILE_HANDLER_H

