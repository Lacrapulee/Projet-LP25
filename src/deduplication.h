#ifndef DEDUPLICATION_H
#define DEDUPLICATION_H
#define HASH_SIZE MD5_DIGEST_LENGTH  // MD5 digest length
#define CHUNK_SIZE 4096             // Chunk size: 4KB

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void deduplicate_files(const char *source, const char *destination, const char *hashes, int v);

#endif // DEDUPLICATION_H

