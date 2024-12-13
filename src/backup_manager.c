#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // Pour mkdir et stat
#include "backup_manager.h"
#include "file_handler.h"
#include "deduplication.h"

// Initialisation du nouveau fichier de sauvegarde en local et du data 
void init_backup(const char * source){
    // Si le dossier backup-source n'existe pas on le créé ainsi que le dossier data, 

    // Sinon 
        // On copie les données du dernier backup, on le renomme avec la date (aaaa/mm/jj/hh/min/ss.ms)
            // pour trouver le dernier back up on récupère les noms et on les tris.
            // pour copier, on fait une commande cp à l'aide de la bib sys 


    // On peut appeler la fonction create backup avec le comme source la source et comme destination le dossier créé.   

    return; // ne pas effacer 
}


void remove_directory(const char *source){
    char *p;
    struct stat st;
    if (stat(source, &st) == 0 && S_ISREG(st.st_mode))
    {
        if (asprintf(&p, "rm -r %s", source) != -1)
        {
            system(p);
            free(p);
        }
    }
    return; 
}

void create_backup(const char *source, const char *destination) {

    // List files
    char **files_src = list_files(source);
    char **files_dest = list_files(destination);

    // Sync files
    for (int i = 0; i < len(files_src); i++) {
        char src_path[PATH_MAX], dest_path[PATH_MAX];
        snprintf(src_path, PATH_MAX, "%s/%s", source, files_src[i]);
        snprintf(dest_path, PATH_MAX, "%s/%s", destination, files_src[i]);

        deduplicate_files(src_path, dest_path);
    }

    // Remove extra files in destination
    for (int i = 0; i < len(files_dest); i++) {
        int found = 0;
        for (int j = 0; j < len(files_src); j++) {
            if (strcmp(files_dest[i], files_src[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char dest_path[PATH_MAX];
            snprintf(dest_path, PATH_MAX, "%s/%s", destination, files_dest[i]);
            remove_directory(dest_path);
        }
    }

    // List folders
    char **folders_src = list_folders(source);
    char **folders_dest = list_folders(destination);

    // Sync folders
    for (int i = 0; i < len(folders_src); i++) {
        char new_src[PATH_MAX], new_dest[PATH_MAX];
        snprintf(new_src, PATH_MAX, "%s/%s", source, folders_src[i]);
        snprintf(new_dest, PATH_MAX, "%s/%s", destination, folders_src[i]);
        // Vérification si l'élément existe déjà dans la destination
        struct stat st;
        if (stat(new_dest, &st) != 0) { 
            // L'élément n'existe pas dans la destination, création nécessaire
            if (mkdir(new_dest) != 0) {
                perror("Erreur lors de la création du répertoire");
                continue; // Passer à l'élément suivant si la création échoue
            }
        }
        // Recursive call for subdirectories
        create_backup(new_src, new_dest);
        
    }

    // Remove extra folders in destination
    for (int i = 0; i < len(folders_dest); i++) {
        int found = 0;
        for (int j = 0; j < len(folders_src); j++) {
            if (strcmp(folders_dest[i], folders_src[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char dest_path[PATH_MAX];
            snprintf(dest_path, PATH_MAX, "%s/%s", destination, folders_dest[i]);
            remove_directory(dest_path);
        }
    }

    // Free allocated memory
    for (int i = 0; i < len(files_src); i++) free(files_src[i]);
    for (int i = 0; i < len(files_dest); i++) free(files_dest[i]);
    for (int i = 0; i < len(folders_src); i++) free(folders_src[i]);
    for (int i = 0; i < len(folders_dest); i++) free(folders_dest[i]);

    free(files_src);
    free(files_dest);
    free(folders_src);
    free(folders_dest);
}
void restore_backup(const char *backup_id, const char *destination) {
    // Implémenter la logique de restauration d'une sauvegarde
    // create_backup() peut-etre ? 
    // Recuperer le dernier fichier en date et le changer. 
}

void list_backup(const char *directory) {
    // Implémenter la logique de liste des sauvegardes
    // recupere le nom du fichier et aller sur dossier de ses backups
}