#include "deduplication.h"
#include "tools.h"
#define HASH_SIZE MD5_DIGEST_LENGTH // Taille du hachage MD5
#define CHUNK_SIZE 4096            // Taille d'un bloc de donnée 4Ko


// Structure pour stocker les chunks et leur hachage
typedef struct {
    unsigned char hash[HASH_SIZE];
    char *filename;
} FileChunk;


// On fait un chunk par ligne

void deduplicate_files(const char *source, const char *destination) {
    // Implémenter la logique de déduplication des fichiers
    
    // Tant que on est pas à la fin du fichier, 

        // prendre le prochain caractère, 
        // Si c'est un \n 
            //changer de chunk et l'envoyer  
        //sinon 
            // on rentre le charactère 


}


