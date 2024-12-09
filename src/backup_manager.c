#include "backup_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

// Initialisation du nouveau fichier de sauvegarde en local 
void init_backup(){
    
}

void copy_files(const char *source, const char *destination) {
    
     
    // on ouvre les deux fichiers, on les fragementent en chunk 

}

void remove_files(const char *source){


    return; 
}


void create_backup(const char *source, const char *destination) {
    

    char **tableau_fichier_source = malloc(50 * sizeof(char *)); 
    char **tableau_fichier_dest = malloc(50 * sizeof(char *)); 

    tableau_fichier_dest = list_files(source);
    tableau_fichier_dest = list_files(destination); 

    for (int i = 0; i < len(tableau_fichier_source); i++){
        // copy_file(source/tableau[i], destination/tableau[i]);
    } 

    // On regarde aussi les fichier en trop dans la destination (qui etait là de base et qui on été effacer dans la source)
    for (int i = 0; i < len(tableau_fichier_dest); i++){
        
        int rm = 1;
        
        for (int j = 0; j < len(tableau_fichier_source); j++){
             
            if (tableau_fichier_dest[i] == tableau_fichier_source[j]){
                rm = 0; 
            }
        }
        if (rm){
            //remove_files(tableau_fichier_dest[i]);
        }  
    }


    // On liste les dossiers de la sources, 
    // Implémenter la logique de création d'une sauvegarde :
    char command_file_source[1024];
    // On liste les fichier dans la source dans la destination 
    snprintf(command_file_source, sizeof(command_file_source), "ls -p %s | grep -v /", source);
    FILE *fp_source;
    char buffer_fichier_source[1024];
    char **tableau_fichier_source = malloc(50 * sizeof(char *)); 
    fp_source = popen(command_file_source, "r");
    int n_fichier_source = 0;
    while (fgets(buffer_fichier_source, sizeof(buffer_fichier_source), fp_source) != NULL) {
    
        buffer_fichier_source[strcspn(buffer_fichier_source, "\n")] = '\0'; // Supprimer le '\n'
        tableau_fichier_source[n_fichier_source] = malloc(strlen(buffer_fichier_source) + 1); // Allouer la mémoire pour la chaîne
        strcpy(tableau_fichier_source[n_fichier_source], buffer_fichier_source);             // Copier le contenu de buffer dans tableau[n]
        printf("%s \n", tableau_fichier_source[n_fichier_source]);
        n_fichier_source++;

    }
        // Si le dossiers n'existe pas dans la destination : 
            // On créé le dossier 
        
        // On execute create_backup(source/dossier, destination/dossier)

    // On liste les dossier de la destination 
        // Si le dossier n'existe pas dans la source, 
            // On le supprime
    
    
    // Libérer la mémoire
    for (int i = 0; i < 50; i++) {
        free(tableau_fichier_dest[i]);
        free(tableau_fichier_source[i]);
    }
    free(tableau_fichier_dest);
    free(tableau_fichier_source);
    return;
}

void restore_backup(const char *backup_id, const char *destination) {
    // Implémenter la logique de restauration d'une sauvegarde

    // Recuperer le dernier fichier en date et le changer. (à voir si on doit utiliser les chunks)
}

void list_backup(const char *directory) {
    // Implémenter la logique de liste des sauvegardes
    // recupere le nom du fichier et aller sur dossier de ses backups
}