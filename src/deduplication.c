#include "deduplication.h"
#include <openssl/md5.h>


void md5_hash_from_string (char *string, char *hash)
{
    int i;
    char unsigned md5[MD5_DIGEST_LENGTH] = {0};

    MD5((const unsigned char *)string, strlen(string), md5);

    for (i=0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2*i, "%02x", md5[i]);
    }
}

// Fonction pour lire les deux fichiers et effectuer la déduplication
// `source` : fichier source à traiter
// `destination` : fichier de destination pour les chunks uniques
// `old_hashes` : fichier pour stocker l'historique des hashes existants
// `new_hashes` : fichier pour stocker les nouveaux hashes générés
// `verbosity` : niveau de verbosité (1 pour afficher les messages d'information)
void deduplicate_files(const char *source, const char *destination, const char *old_hashes, const char *new_hashes, int verbosity) {
    FILE *srcFile, *destFile, *newHashFile, *oldHashFile;
    unsigned char buffer[CHUNK_SIZE];
    unsigned char currentHash[2*HASH_SIZE+1];
    unsigned char hashBuffer[2*HASH_SIZE+1];
    size_t bytesRead;
    int isDuplicate;
    long chunkPosition = 0;  // Compteur pour déterminer la position à chaque chunk

    // Si la verbosité est activée, afficher les informations de début
    if (verbosity == 1) {
        printf("[INFO] Démarrage de la déduplication : source='%s', destination='%s', old_hashes='%s', new_hashes='%s'\n", source, destination, old_hashes, new_hashes);
    }

    // Ouvrir le fichier source en mode binaire pour lecture
    srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier source");
        return;
    }

    // Ouvrir le fichier des anciens hashes en mode lecture
    oldHashFile = fopen(old_hashes, "rb");
    if (oldHashFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier des anciens hashes");
        fclose(srcFile);
        return;
    }

    // Ouvrir le fichier des nouveaux hashes en mode ajout binaire
    if (new_hashes != NULL) {
        newHashFile = fopen(new_hashes, "a+b");
        if (newHashFile == NULL) {
            perror("Erreur lors de l'ouverture du fichier des nouveaux hashes");
            fclose(srcFile);
            fclose(oldHashFile);
            return;
        }
    }

    // Créer le fichier de destination s'il n'existe pas
    destFile = fopen(destination, "a");
    fclose(destFile);
    // Ouvrir le fichier de destination en mode lecture/écriture binaire
    destFile = fopen(destination, "r+b");
    if (destFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de destination");
        fclose(srcFile);
        fclose(oldHashFile);
        fclose(newHashFile);
        return;
    }

    // Lire le fichier source par chunks
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, srcFile)) > 0) {
        // Calculer le hash MD5 du chunk
        md5_hash_from_string(buffer, currentHash);
        // Vérifier si ce hash existe déjà dans le fichier des anciens hashes
        fseek(oldHashFile, 0, SEEK_SET);
        isDuplicate = 0;
        while (fread(hashBuffer, 1, 2*HASH_SIZE+1, oldHashFile) == 2*HASH_SIZE+1) {
            if (memcmp(hashBuffer, currentHash, 2*HASH_SIZE+1) == 0) {
                isDuplicate = 1;  // Le chunk est un doublon
                if (verbosity == 1) {
                    printf("[INFO] Chunk détecté comme doublon (trouvé dans les anciens hashes).\n");
                }
                break;
            }
        }

        // Si le chunk n'est pas un doublon, il faut l'ajouter ou le remplacer
        if (!isDuplicate) {
            // Déplacer le curseur à la position du chunk actuel dans le fichier destination
            fseek(destFile, chunkPosition, SEEK_SET);
            // Remplacer ou écrire le chunk à la position calculée
            fwrite(buffer, 1, bytesRead, destFile);
            if (verbosity == 1) {
                printf("[INFO] Chunk écrit/remplacé à la position %ld dans le fichier de destination.\n", chunkPosition);
            }

        } else {
            if (verbosity == 1) {
                printf("[INFO] Chunk ignoré car il existe déjà dans l'historique.\n");
            }
        }
        // Sauvegarder le hash dans le fichier des nouveaux hashes
        if (new_hashes != NULL) {
            fwrite(currentHash, 1, 2*HASH_SIZE+1, newHashFile);
        }
        // Incrémenter la position pour le prochain chunk
        chunkPosition += bytesRead;
    }

    // Si la verbosité est activée, afficher les informations de fin
    if (verbosity == 1) {
        printf("[INFO] Déduplication terminée pour '%s'.\n", source);
    }

    // Fermer les fichiers ouverts
    fclose(srcFile);
    fclose(oldHashFile);
    fclose(destFile);
    fclose(newHashFile);
}

// Fonction pour restaurer un fichier à partir des chunks sauvegardés
// `file_to_restore` : chemin du fichier à restaurer
// `dedup_file` : chemin du fichier contenant les chunks sauvegardés
// `hashes` : chemin du fichier contenant les hashes de l'historique
void restore_file(const char *file_to_restore, const char *dedup_file, const char *hashes, int v) {
    FILE *restoreFile, *dedupFile, *hashFile, *test;
    unsigned char buffer[CHUNK_SIZE] = "";
    unsigned char buffer2[CHUNK_SIZE] = "";
    unsigned char hashBuffer[2*HASH_SIZE+1] = "";
    unsigned char dedupHash[2*HASH_SIZE+1] = "";
    unsigned char restoreHash[2*HASH_SIZE+1] ="";
    long chunkPosition = 0;  // Compteur pour déterminer la position à chaque chunk
    size_t bytesRead;
    size_t bytesRead2;
    int isDuplicate;

    // Afficher un message d'information si le niveau de verbosité est 1
    if (v == 1) printf("[INFO] Début de la restauration : file_to_restore='%s', dedup_file='%s', hashes='%s'\n", file_to_restore, dedup_file, hashes);
    
    
    // Créer le fichier de restoration s'il n'existe pas
    restoreFile = fopen(file_to_restore, "a");
    fclose(restoreFile);
    // Ouvrir le fichier de restauration en mode lecture/écriture binaire
    restoreFile = fopen(file_to_restore, "r+b");
    if (restoreFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier à restaurer");
        return;
    }
    // Ouvrir le fichier de restauration en mode lecture/écriture binaire
    test = fopen(file_to_restore, "r+b");
    if (test == NULL) {
        perror("Erreur lors de l'ouverture du fichier à restaurer");
        fclose(restoreFile);

        return;
    }

    // Ouvrir le fichier contenant les chunks dédupliqués en mode lecture binaire
    dedupFile = fopen(dedup_file, "rb");
    if (dedupFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier des chunks dédupliqués");
        fclose(restoreFile);
        fclose(test);

        return;
    }

    // Ouvrir le fichier des hashes en mode lecture
    hashFile = fopen(hashes, "rb");
    if (hashFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier des hashes");
        fclose(restoreFile);
        fclose(dedupFile);
        fclose(test);
        return;
    }

    // Parcourir le fichier à restaurer chunk par chunk
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, restoreFile)) > 0) {
        
        // Calculer le hash MD5 du chunk courant du fichier à restaurer
        md5_hash_from_string(buffer, restoreHash);
        // Vérifier si ce hash existe déjà dans le fichier des hashes
        fseek(hashFile, 0, SEEK_SET);
        isDuplicate = 0;
        while (fread(hashBuffer, 1, 2*HASH_SIZE+1, hashFile) == 2*HASH_SIZE+1) {
            
            if (memcmp(hashBuffer, restoreHash, 2*HASH_SIZE+1) == 0) {
                isDuplicate = 1; // Le chunk est déjà dans les hashes
                if (v == 1) printf("[INFO] Chunk déjà présent dans l'historique, ignoré.\n");
                break;
            }
        }
        
        // Si le chunk n'est pas un doublon, rechercher le chunk dans le fichier dédupliqué
        if (!isDuplicate) {
            fseek(dedupFile, 0, SEEK_SET);
            int found = 0;
            while ((bytesRead2 = fread(buffer2, 1, CHUNK_SIZE, dedupFile)) > 0) {
                md5_hash_from_string(buffer2, dedupHash);
                fseek(hashFile, 0, SEEK_SET);//On reset le pointeur de lecture 
                while (fread(hashBuffer, 1, 2*HASH_SIZE+1, hashFile) == 2*HASH_SIZE+1) {
                    if (memcmp(hashBuffer, dedupHash, 2*HASH_SIZE+1) == 0) {
                        found = 1;
                        if (v == 1) printf("[INFO] Chunk trouvé dans le fichier dédupliqué, ajout au fichier restauré.\n");
                        
                        // Écrire le chunk dans le fichier restauré
                        // Ouvrir le fichier de restauration en mode écriture binaire
                        printf("position : %ld\n", chunkPosition);
                        printf("%s\n", buffer2);
                        fseek(test, chunkPosition, SEEK_END);
                        fwrite(buffer2, 1, bytesRead2, test);
                        break;
                    }
                }
            }

            if (!found) {
                fprintf(stderr, "[ERREUR] Chunk correspondant non trouvé dans le fichier dédupliqué pour le hash calculé.\n");
            }

            
        }
        chunkPosition += bytesRead;
    }

    // Afficher un message de fin si le niveau de verbosité est 1
    if (v == 1) printf("[INFO] Restauration terminée pour '%s'.\n", file_to_restore);

    // Fermer les fichiers ouverts
    fclose(restoreFile);
    fclose(dedupFile);
    fclose(hashFile);
    fclose(test);
}
