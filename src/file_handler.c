#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "file_handler.h"
/*
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
*/



int len(char ** tableau){
    int i = 0; 

    while (tableau[i] != "\0"){
        i++;
    }
    return i;
}

char **list_files(const char *path) {
    // Open the directory
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    // Allocate space for 50 file names (adjust size as needed)
    char **tableau = malloc(50 * sizeof(char *));
    if (tableau == NULL) {
        perror("malloc");
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    int i = 0;

    // Read and store the file names in tableau
    while ((entry = readdir(dir)) != NULL) {
        tableau[i] = malloc(strlen(entry->d_name) + 1); // Allocate space for the file name
        if (tableau[i] == NULL) {
            perror("malloc");
            // Free the previously allocated memory
            for (int j = 0; j < i; j++) {
                free(tableau[j]);
            }
            free(tableau);
            closedir(dir);
            return NULL;
        }
        strcpy(tableau[i], entry->d_name); // Copy the file name to the allocated memory
        i++;
    }

    // Close the directory
    closedir(dir);

    // Return the list of file names
    return tableau;
}

char** list_folder(const char *path) {
    
    char command[1024];
    // On liste les fichier dans la source dans la destination 
    snprintf(command, sizeof(command), "ls --directory %s/*/ | xargs -n 1 basename", path);
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
