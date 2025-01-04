#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "deduplication.h"
#include <stdio.h>


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

// Fonction pour lire les deux fichier et les copier 
void deduplicate_files(const char *source, const char *destination, const char *hashes) {
    FILE *srcFile, *destFile;
    unsigned char buffer[CHUNK_SIZE];
    unsigned char hash[HASH_SIZE];
    size_t bytesRead;
    FILE *hashFile;
    int isDuplicate;
    size_t i, j;
    char dirpath[1024];   // Pour stocker le chemin du répertoire destination

    // Copier destination pour éviter de modifier l'original
    strncpy(dirpath, destination, sizeof(dirpath));
    dirpath[sizeof(dirpath) - 1] = '\0'; // Sécuriser la chaîne

    // Trouver le dernier '/' pour isoler le répertoire
    char *last_slash = strrchr(dirpath, '/');
    if (last_slash != NULL) {
        *last_slash = '\0'; // Couper la chaîne pour ne garder que le répertoire
    } else {
        fprintf(stderr, "Chemin invalide : pas de répertoire trouvé.\n");
        return; // Erreur si le chemin n'a pas de '/'
    }

    // on ouvre le fichier source en lecture binaire 
    srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        perror("Error opening source file");
        return;
    }

    // Ouvre le fichier destinaton en écriture 
    destFile = fopen(destination, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        return;
    }

    // Créé un fichier pour l'historique des chunks 
    hashFile = fopen(hashes, "a+b");
    if (hashFile == NULL) {
        perror("Error opening hash storage file");
        fclose(srcFile);
        fclose(destFile);
        return;
    }

    // Découpe de fichier en chunk 
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, srcFile)) > 0) {
        // faire le code md5 du chunk
        MD5(buffer, bytesRead, hash);

        // regarde si le chunk n'est pas dans le fichier historique 
        fseek(hashFile, 0, SEEK_SET);
        isDuplicate = 0;
        while (fread(buffer, 1, HASH_SIZE, hashFile) == HASH_SIZE) {
            if (memcmp(buffer, hash, HASH_SIZE) == 0) {
                isDuplicate = 1;
                break;
            }
        }

        // si il est double, on passe 
        if (isDuplicate) {
            printf("Double, on passe.\n");
        } else {
            // Sinon on ecrit le chunk dans la destination
            fwrite(buffer, 1, bytesRead, destFile);

            // on sauvegarde dans l'historique 
            fwrite(hash, 1, HASH_SIZE, hashFile);
        }
    }

    
    fclose(srcFile);
    fclose(destFile);
    fclose(hashFile);
}
