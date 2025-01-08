#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H
#define MAX_PATH 1024
#include <stdio.h>
#include "file_handler.h"
#include "deduplication.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>

void init_backup(const char *backup_dir, const char *source, int verbose);
void create_backup(const char *source, const char *destination, const char *old_hashes_file, const char *new_hashes_file, int verbose);
void restore_backup(const char *backup_id, const char *source, const char *destination, const char *hashes, int verbose);


#endif // BACKUP_MANAGER_H

