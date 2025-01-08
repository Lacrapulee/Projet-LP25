#ifndef DEDUPLICATION_H
#define DEDUPLICATION_H

<<<<<<< Updated upstream
void deduplicate_files(const char *source, const char *destination);
=======
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void restore_file(const char *file_to_restore, const char *dedup_file, const char *hashes, int v);
void deduplicate_files(const char *source, const char *destination, const char *old_hashes_file, const char *new_hashes_file, int v);
>>>>>>> Stashed changes

#endif // DEDUPLICATION_H

