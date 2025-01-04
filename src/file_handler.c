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



// The list_files function, which returns an array of file names in a directory
char **list_files(const char *path) {
    // Open the directory
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    // Allocate space for file names (initial size)
    size_t capacity = 50;
    char **tableau = malloc(capacity * sizeof(char *));
    if (tableau == NULL) {
        perror("malloc");
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    int i = 0;

    // Read and store the file names in tableau
    while ((entry = readdir(dir)) != NULL) {
        // Ignore directories like "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Resize tableau if needed
        if (i >= capacity) {
            capacity *= 2;  // Double the size
            tableau = realloc(tableau, capacity * sizeof(char *));
            if (tableau == NULL) {
                perror("realloc");
                closedir(dir);
                return NULL;
            }
        }

        tableau[i] = malloc(strlen(entry->d_name) + 1); // Allocate space for the file name
        if (tableau[i] == NULL) {
            perror("malloc");
            // Free previously allocated memory
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

    // Mark the end of the array
    tableau[i] = NULL;

    return tableau;
}
char **list_folder(const char *path) {
    char command[1024];
    snprintf(command, sizeof(command), "ls --directory %s/*/ | xargs -n 1 basename", path);

    FILE *fp;
    char buffer[1024];
    size_t capacity = 50;
    char **tableau = malloc(capacity * sizeof(char *));
    if (tableau == NULL) {
        perror("malloc");
        return NULL;
    }

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        free(tableau);
        return NULL;
    }

    int n = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Remove the newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Resize tableau if needed
        if (n >= capacity) {
            capacity *= 2;  // Double the size
            tableau = realloc(tableau, capacity * sizeof(char *));
            if (tableau == NULL) {
                perror("realloc");
                fclose(fp);
                return NULL;
            }
        }

        tableau[n] = malloc(strlen(buffer) + 1);  // Allocate memory for the directory name
        if (tableau[n] == NULL) {
            perror("malloc");
            // Free the previously allocated memory
            for (int i = 0; i < n; i++) {
                free(tableau[i]);
            }
            free(tableau);
            fclose(fp);
            return NULL;
        }
        strcpy(tableau[n], buffer);  // Copy the directory name
        n++;
    }

    // Mark the end of the array
    tableau[n] = NULL;

    fclose(fp);
    return tableau;
}
