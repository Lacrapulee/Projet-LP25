#include <stdio.h>
#include "file_handler.h"
#include "tools.h"


void list_files(const char *path) {
    // Implémenter la logique pour lister les fichiers dans le répertoire donné
    // faire un ls sur le dossier, faire list_files(*path + le dossier) quand c'est un dossier ou alors si c'est un fichier le mettre dans la liste que l'on retourne. 
    // S'inspirer de la fonction create_backup() 
}

void read_file(const char *filepath) {
    // Implémenter la logique pour lire un fichier
}

void write_file(const char *filepath, const void *data, size_t size) {
    // Implémenter la logique pour écrire des données dans un fichier
}



char** list_files(const char *path) {
    
    char command[1024];
    // On liste les fichier dans la source dans la destination 
    snprintf(command, sizeof(command), "ls -p %s | grep -v /", path);
    FILE *fp;
    char buffer[1024];
    char **tableau = malloc(50 * sizeof(char *)); 
    fp = popen(command, "r");
    int n = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le '\n'
        tableau[n] = malloc(strlen(buffer) + 1); // Allouer la mémoire pour la chaîne
        strcpy(tableau[n], buffer);             // Copier le contenu de buffer dans tableau[n]
        printf("%s \n", tableau[n]);
        n++;

    }
    return tableau; 
}

char** list_folder(const char *path) {
    
    char command[1024];
    // On liste les fichier dans la source dans la destination 
    snprintf(command, sizeof(command), "ls --directory %d/*/ | xargs -n 1 basename", path);
    FILE *fp;
    char buffer[1024];
    char **tableau = malloc(50 * sizeof(char *)); 
    fp = popen(command, "r");
    int n = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le '\n'
        tableau[n] = malloc(strlen(buffer) + 1); // Allouer la mémoire pour la chaîne
        strcpy(tableau[n], buffer);             // Copier le contenu de buffer dans tableau[n]
        printf("%s \n", tableau[n]);
        n++;

    }
    return tableau; 
}
