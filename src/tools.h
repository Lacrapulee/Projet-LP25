#ifndef TOOLS
#define TOOLS
#define _GNU_SOURCE  // Pour activer asprintf
#include "file_handler.h"
#define MAX_PATH 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ftw.h>  // Pour suppression récursive des répertoires

int is_directory(const char *path); 
int copy_item(const char *source, const char *destination); 
void create_directory(const char *path); 
void get_current_time(char *buffer, size_t size); 
char *find_most_recent_folder(const char *directory); 
int array_length(char **array); 
int copy_with_cp(const char *source, const char *destination); 
void remove_directory(const char *path);
void list_backups(const char *dir_backup, int verbose, int i); 
void handle_dry_run(const char *source, const char *destination, const char *backup_id, int back_up);

#endif // TOOLS