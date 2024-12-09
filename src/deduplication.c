#include "deduplication.h"
#include "tools.h"
#include <stdio.h>
#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_SIZE MD5_DIGEST_LENGTH  // MD5 digest length
#define CHUNK_SIZE 4096             // Chunk size: 4KB

typedef struct {
    unsigned char hash[HASH_SIZE];
    char *filename;
} FileChunk;

// Function to compute the MD5 hash of a string
void md5_hash_from_string(char *string, char *hash) {
    int i;
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};

    MD5((const unsigned char *)string, strlen(string), md5);

    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }
}

// Compare two MD5 hashes
int compare_hashes(const void *a, const void *b) {
    return memcmp(a, b, HASH_SIZE);
}

// Function to read the file in chunks and deduplicate
void deduplicate_files(const char *source, const char *destination) {
    FILE *srcFile, *destFile;
    unsigned char buffer[CHUNK_SIZE];
    unsigned char hash[HASH_SIZE];
    size_t bytesRead;
    FILE *hashFile;
    int isDuplicate;
    size_t i, j;

    // Open source file for reading
    srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        perror("Error opening source file");
        return;
    }

    // Open destination file for writing
    destFile = fopen(destination, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        return;
    }

    // Create a file to store seen hashes
    hashFile = fopen("hashes.dat", "a+b");
    if (hashFile == NULL) {
        perror("Error opening hash storage file");
        fclose(srcFile);
        fclose(destFile);
        return;
    }

    // Read the source file in chunks
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, srcFile)) > 0) {
        // Compute the MD5 hash of the chunk
        MD5(buffer, bytesRead, hash);

        // Check if the hash is already in the hash storage
        fseek(hashFile, 0, SEEK_SET);
        isDuplicate = 0;
        while (fread(buffer, 1, HASH_SIZE, hashFile) == HASH_SIZE) {
            if (memcmp(buffer, hash, HASH_SIZE) == 0) {
                isDuplicate = 1;
                break;
            }
        }

        // If the hash is a duplicate, skip writing this chunk to the destination
        if (isDuplicate) {
            printf("Duplicate chunk found, skipping.\n");
        } else {
            // Write the chunk to the destination file
            fwrite(buffer, 1, bytesRead, destFile);

            // Save the new hash to the hash storage
            fwrite(hash, 1, HASH_SIZE, hashFile);
        }
    }

    // Close all files
    fclose(srcFile);
    fclose(destFile);
    fclose(hashFile);
}
