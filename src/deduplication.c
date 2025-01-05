#include "deduplication.h"
#include <openssl/md5.h>

// Fonction pour calculer le hash MD5 à partir d'une chaîne de caractères
// `string` : chaîne à hasher
// `hash` : tampon pour stocker le hash MD5
void md5_hash_from_string(char *string, char *hash) {
    int i;
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};

    // Calculer le hash MD5 de la chaîne
    MD5((const unsigned char *)string, strlen(string), md5);

    // Convertir le tableau md5 en une chaîne hexadécimale
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }
}

// Fonction pour comparer deux hashes MD5
// `a` et `b` : pointeurs vers les deux hashes à comparer
// Retourne 0 si les hashes sont égaux, un autre nombre sinon
int compare_hashes(const void *a, const void *b) {
    return memcmp(a, b, HASH_SIZE);
}

// Fonction pour lire les deux fichiers et effectuer la déduplication
// `source` : fichier source à traiter
// `destination` : fichier de destination pour les chunks uniques
// `hashes` : fichier pour stocker l'historique des hashes
// `v` : niveau de verbosité (1 pour afficher les messages d'information)
void deduplicate_files(const char *source, const char *destination, const char *hashes, int v) {
    FILE *srcFile, *destFile;
    unsigned char buffer[CHUNK_SIZE];
    unsigned char hash[HASH_SIZE];
    unsigned char hashBuffer[HASH_SIZE]; // Nouveau tampon pour les hashes
    size_t bytesRead;
    FILE *hashFile;
    int isDuplicate;

    // Afficher le message d'information au début si le niveau de verbosité est 1
    if (v == 1) printf("[INFO] Début de la déduplication : source='%s', destination='%s', hashes='%s'\n", source, destination, hashes);

    // Ouvrir le fichier source en mode binaire pour lecture
    srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier source");
        return;
    }

    // Ouvrir le fichier des hashes (historique) en mode ajout
    hashFile = fopen(hashes, "a+b");
    if (hashFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de stockage des hashes");
        fclose(srcFile);
        return;
    }

    // Découper le fichier source en morceaux (chunks)
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, srcFile)) > 0) {
        // Calculer le hash MD5 du chunk
        MD5(buffer, bytesRead, hash);

        // Vérifier si ce hash existe déjà dans l'historique
        fseek(hashFile, 0, SEEK_SET);
        isDuplicate = 0;
        while (fread(hashBuffer, 1, HASH_SIZE, hashFile) == HASH_SIZE) {
            if (memcmp(hashBuffer, hash, HASH_SIZE) == 0) {
                isDuplicate = 1;  // Le chunk est un doublon
                if (v == 1) printf("[INFO] Chunk détecté comme doublon (hash trouvé dans l'historique).\n");
                break;
            }
        }

        // Si le chunk n'est pas un doublon, l'ajouter au fichier destination
        if (!isDuplicate) {
            if (v == 1) printf("[INFO] Nouveau chunk détecté, ajout au fichier de destination.\n");

            // Ouvrir le fichier de destination en mode ajout
            destFile = fopen(destination, "ab");
            if (destFile == NULL) {
                perror("Erreur lors de l'ouverture du fichier de destination");
                fclose(srcFile);
                fclose(hashFile);
                return;
            }

            // Écrire le chunk dans le fichier de destination
            fwrite(buffer, 1, bytesRead, destFile);
            fclose(destFile);

            // Sauvegarder le hash du chunk dans le fichier des hashes
            fwrite(hash, 1, HASH_SIZE, hashFile);
            if (v == 1) printf("[INFO] Chunk sauvegardé dans l'historique des hashes.\n");
        } else {
            if (v == 1) printf("[INFO] Chunk ignoré car déjà existant dans l'historique.\n");
        }
    }

    // Afficher un message à la fin si le niveau de verbosité est 1
    if (v == 1) printf("[INFO] Fin de la déduplication pour '%s'.\n", source);

    // Fermer les fichiers ouverts
    fclose(srcFile);
    fclose(hashFile);
}