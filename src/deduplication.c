#include "deduplication.h"
#include "tools.h"
#define HASH_SIZE MD5_DIGEST_LENGTH // Taille du hachage MD5
#define CHUNK_SIZE 4096            // Taille d'un bloc de donnée 4Ko


// Structure pour stocker les chunks et leur hachage
typedef struct {
    unsigned char hash[HASH_SIZE];
    char *filename;#include "deduplication.h"

// Fonction de hachage MD5 pour l'indexation dans la table de hachage
unsigned int hash_md5(unsigned char *md5) {
    unsigned int hash = 0;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        hash = (hash << 5) + hash + md5[i];
    }
    return hash % HASH_TABLE_SIZE;
}

// Fonction pour calculer le MD5 d'un chunk
void compute_md5(void *data, size_t len, unsigned char *md5_out) {
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, data, len);
    MD5_Final(md5_out, &md5_ctx);
}

// Fonction permettant de chercher un MD5 dans la table de hachage
int find_md5(Md5Entry *hash_table, unsigned char *md5) {
    unsigned int index = hash_md5(md5);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        int probe_index = (index + i) % HASH_TABLE_SIZE;
        if (hash_table[probe_index].index == -1) {
            return -1; // Entrée vide, MD5 non trouvé
        }
        if (memcmp(hash_table[probe_index].md5, md5, MD5_DIGEST_LENGTH) == 0) {
            return hash_table[probe_index].index; // MD5 trouvé
        }
    }
    return -1; // MD5 non trouvé
}

// Ajouter un MD5 dans la table de hachage
void add_md5(Md5Entry *hash_table, unsigned char *md5, int index) {
    unsigned int hash_index = hash_md5(md5);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        int probe_index = (hash_index + i) % HASH_TABLE_SIZE;
        if (hash_table[probe_index].index == -1) {
            // Trouvé une case vide
            memcpy(hash_table[probe_index].md5, md5, MD5_DIGEST_LENGTH);
            hash_table[probe_index].index = index;
            return;
        }
    }
    fprintf(stderr, "Erreur : Table de hachage pleine.\n");
    exit(EXIT_FAILURE);
}

// Fonction pour convertir un fichier non dédupliqué en tableau de chunks
void deduplicate_file(FILE *file, Chunk *chunks, Md5Entry *hash_table) {
    unsigned char buffer[CHUNK_SIZE];
    size_t bytes_read;
    int chunk_index = 0;

    // Initialiser la table de hachage
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hash_table[i].index = -1;
    }

    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        unsigned char md5[MD5_DIGEST_LENGTH];
        compute_md5(buffer, bytes_read, md5);

        int existing_index = find_md5(hash_table, md5);
        if (existing_index == -1) {
            // Chunk unique, on l'ajoute
            add_md5(hash_table, md5, chunk_index);

            // Ajouter le chunk au tableau
            chunks[chunk_index].data = malloc(bytes_read);
            if (chunks[chunk_index].data == NULL) {
                perror("Erreur d'allocation mémoire");
                exit(EXIT_FAILURE);
            }
            memcpy(chunks[chunk_index].data, buffer, bytes_read);
            memcpy(chunks[chunk_index].md5, md5, MD5_DIGEST_LENGTH);

            chunk_index++;
        }
        // Sinon, le chunk est déjà présent, pas besoin de l'ajouter
    }
}

// Fonction permettant de charger un fichier dédupliqué en table de chunks
// en remplaçant les références par les données correspondantes
void undeduplicate_file(FILE *file, Chunk **chunks, int *chunk_count) {
    fread(chunk_count, sizeof(int), 1, file);
    *chunks = malloc(sizeof(Chunk) * (*chunk_count));
    if (*chunks == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *chunk_count; i++) {
        size_t data_size;
        fread(&data_size, sizeof(size_t), 1, file);

        (*chunks)[i].data = malloc(data_size);
        if ((*chunks)[i].data == NULL) {
            perror("Erreur d'allocation mémoire");
            exit(EXIT_FAILURE);
        }
        fread((*chunks)[i].data, 1, data_size, file);
        fread((*chunks)[i].md5, MD5_DIGEST_LENGTH, 1, file);
    }
}

} FileChunk;

void deduplicate_files(const char *source, const char *destination) {
    // Implémenter la logique de déduplication des fichiers
}


