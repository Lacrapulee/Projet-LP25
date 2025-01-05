#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H
#define MAX_PATH 1024
#include <stdio.h>
#include "file_handler.h"
#include "deduplication.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>


void create_backup(const char *source, const char *destination, const char *hashes, int v);
void init_backup(const char *dir_backup, const char *source, int v);
void restore_backup(const char *backup_id, const char *source, const char *dest, const char *hashes, int v);


#endif // BACKUP_MANAGER_H

