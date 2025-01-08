#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H
<<<<<<< Updated upstream

void create_backup(const char *source, const char *destination);
void restore_backup(const char *backup_id, const char *destination);
void list_backup(const char *directory);

=======
#define MAX_PATH 1024
#include <stdio.h>
#include "file_handler.h"
#include "deduplication.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>


void create_backup(const char *source, const char *destination, const char *old_hashes_file, const char *new_hashes_file,int v);
void init_backup(const char *dir_backup, const char *source, int v);
void restore_backup(const char *backup_id, const char *source, const char *dest, const char *hashes, int v);

>>>>>>> Stashed changes
#endif // BACKUP_MANAGER_H

