#include "backup_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void init_backup(){
    // Initialisation du fichier de sauvegarde en local 
}

void copy_files(const char *source, const char *destination) {
    
    // On compare les chunks ect... 
}

void create_backup(const char *source, const char *destination) {
    
    // Implémenter la logique de création d'une sauvegarde :
    char command_file_source[1024];
    // On liste les fichier dans la source dans la destination 
    snprintf(command_file_source, sizeof(command_file_source), "ls -p %s | grep -v /", source);
    FILE *fp;
    char buffer[1024];
    char **tableau = malloc(50 * sizeof(char *)); 
    fp = popen(command_file_source, "r");
    int n = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le '\n'
        tableau[n] = malloc(strlen(buffer) + 1); // Allouer la mémoire pour la chaîne
        strcpy(tableau[n], buffer);             // Copier le contenu de buffer dans tableau[n]
            printf("%s \n", tableau[n]);

        n++;
    }
    
    
    // On liste les fichier dans la destination 
    






    //on compare les chunks, et on remplace...
    for (int i = 0; i < n; i++){
        // copy_file(source/tableau[i], destination/tableau[i]);
    } 

    // On regarde aussi les fichier en trop dans la destination (qui etait là de base et qui on été effacer dans la source)
    //pclise(fp); pour la destination
    pclose(fp);
    // On liste les dossiers de la sources, 

        // Si le dossiers n'existe pas dans la destination : 
            // On créé le dossier 
        
        // On execute create_backup(source/dossier, destination/dossier)

    // On liste les dossier de la destination 
        // Si le dossier n'existe pas dans la source, 
            // On le supprime
    
    
    // Libérer la mémoire
    for (int i = 0; i < 50; i++) {
        free(tableau[i]);
    }
    free(tableau);
    return;
}

void restore_backup(const char *backup_id, const char *destination) {
    // Implémenter la logique de restauration d'une sauvegarde

    // Recuperer le dernier fichier en date et le changer. (à voir si on doit utiliser les chunks)
}

void list_backup(const char *directory) {
    // Implémenter la logique de liste des sauvegardes
    // recupere le nom du fichier et aller sur dossier de ses backup 
}

