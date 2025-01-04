#include "backup_manager.h"
#include "tools.h"


void create_backup(const char *source, const char *destination) {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    // Implémenter la logique de création d'une sauvegarde
=======
=======
>>>>>>> Stashed changes
    
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
    
    // On liste les fichier dans la destination 
    char command_file_dest[1024];
    snprintf(command_file_dest, sizeof(command_file_dest), "ls -p %s | grep -v /", destination);
    FILE *fp_dest;
    char buffer_fichier_dest[1024];
    char **tableau_fichier_dest = malloc(50 * sizeof(char *)); 
    fp_dest = popen(command_file_dest, "r");
    int n_fichier_dest = 0;
    while (fgets(buffer_fichier_dest, sizeof(buffer_fichier_dest), fp_dest) != NULL) {
    
        buffer_fichier_dest[strcspn(buffer_fichier_dest, "\n")] = '\0'; // Supprimer le '\n'
        tableau_fichier_dest[n_fichier_dest] = malloc(strlen(buffer_fichier_dest) + 1); // Allouer la mémoire pour la chaîne
        strcpy(tableau_fichier_dest[n_fichier_dest], buffer_fichier_dest);             // Copier le contenu de buffer dans tableau[n]
            printf("%s \n", tableau_fichier_dest[n_fichier_dest]);

        n_fichier_dest++;
    }
    
    //on compare les chunks, et on remplace...
    for (int i = 0; i < n_fichier_source; i++){
        // copy_file(source/tableau[i], destination/tableau[i]);
    } 

    // On regarde aussi les fichier en trop dans la destination (qui etait là de base et qui on été effacer dans la source)
    
    for (int i = 0; i < n_fichier_dest; i++){
        int bool = 0;
        for (int j = 0; i < n_fichier_source; i++){
            if (tableau_fichier_dest[i] == tableau_fichier_source[j]){
                bool = 1; // le fichier est bien présent dans les deux répertoire 
            }
        }
        if (bool == 0){
            // Le fichier n'est pas dans la source alors on le retire : 
            FILE *fp;
            char command[1024]; 
            snprintf(command, sizeof(command), "rm %s/%s", destination, tableau_fichier_dest[i]);
            fp = popen(command, "r");


        }
    } 

    pclose(fp_dest); 
    pclose(fp_source);


    // On liste les dossiers de la sources, ls -d */ 

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
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}

void restore_backup(const char *backup_id, const char *destination) {
    // Implémenter la logique de restauration d'une sauvegarde
}

void list_backup(const char *directory) {
    // Implémenter la logique de liste des sauvegardes
}

