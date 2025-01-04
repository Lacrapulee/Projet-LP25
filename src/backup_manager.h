#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

void create_backup(const char *source, const char *destination, const char *hashes);
void restore_backup(const char *backup_id, const char *destination);
void list_backup(const char *directory);
void init_backup(const char *dir_backup, const char *source);
#endif // BACKUP_MANAGER_H

