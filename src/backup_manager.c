#define _GNU_SOURCE  // Pour activer asprintf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <ftw.h>  // Pour suppression récursive des répertoires
#include "file_handler.h"
#include "deduplication.h"

#define MAX_PATH 1024

// Déclarations des fonctions
void create_backup(const char *source, const char *destination, const char *hashes);
void create_directory(const char *path);
void get_current_time(char *buffer, size_t size);
char *find_most_recent_folder(const char *directory);
int array_length(char **array);
int copy_with_cp(const char *source, const char *destination);
int remove_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);
void remove_directory(const char *path);
void init_backup(const char *dir_backup, const char *source);
int is_directory(const char *path);
int copy_item(const char *source, const char *destination);

// Fonction pour créer un dossier si inexistant
void create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) == -1) {
            perror("Erreur lors de la création du répertoire");
        }
    }
}

// Fonction pour récupérer l'heure actuelle formatée
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y.%m.%d.%H.%M.%S", t);
}

// Fonction pour trouver le dossier le plus récent dans un répertoire
char *find_most_recent_folder(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    time_t latest_time = 0;
    static char latest_folder[MAX_PATH] = "";

    if ((dir = opendir(directory)) == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            if (statbuf.st_mtime > latest_time) {
                latest_time = statbuf.st_mtime;
                strncpy(latest_folder, path, sizeof(latest_folder) - 1);
            }
        }
    }

    closedir(dir);
    return latest_folder[0] != '\0' ? latest_folder : NULL;
}

// Fonction pour compter les éléments dans un tableau dynamique
int array_length(char **array) {
    int count = 0;
    while (array[count] != NULL) {
        count++;
    }
    return count;
}

// Fonction pour copier des fichiers/répertoires avec la commande 'cp'
int copy_with_cp(const char *source, const char *destination) {
    char command[4096];
    snprintf(command, sizeof(command), "cp -r '%s'/* '%s'", source, destination);

    int result = system(command);

    if (result != 0) {
        fprintf(stderr, "Erreur : impossible de copier '%s' vers '%s' avec cp.\n", source, destination);
        return -1;
    }

    return 0;
}

// Fonction pour supprimer un fichier ou répertoire récursivement
int remove_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    if (typeflag == FTW_D) {
        return rmdir(fpath);  // Supprime les répertoires vides
    }
    return remove(fpath);  // Supprime les fichiers
}

void remove_directory(const char *path) {
    nftw(path, remove_callback, 64, FTW_DEPTH | FTW_PHYS);
}

// Initialisation du backup
void init_backup(const char *dir_backup, const char *source) {
    char *last_folder = find_most_recent_folder(dir_backup);

    char timestamped_backup_name[MAX_PATH];
    get_current_time(timestamped_backup_name, sizeof(timestamped_backup_name));

    char new_backup_dir[MAX_PATH];
    if (snprintf(new_backup_dir, MAX_PATH, "%s/%s", dir_backup, timestamped_backup_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin de backup trop long.\n");
        return;
    }
    create_directory(new_backup_dir);

    char hashes_file[MAX_PATH];
    if (snprintf(hashes_file, MAX_PATH, "%s/hashes.dat", dir_backup) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin du fichier hashes trop long.\n");
        return;
    }

    FILE *hash_file = fopen(hashes_file, "a");
    if (hash_file == NULL) {
        perror("Erreur lors de la création du fichier 'hashes.dat'");
        return;
    }
    fclose(hash_file);

    if (last_folder) {
        if (copy_with_cp(last_folder, new_backup_dir) == 0) {
            printf("Copie réussie de '%s' vers '%s'.\n", last_folder, new_backup_dir);
        } else {
            fprintf(stderr, "Erreur lors de la copie depuis '%s'.\n", last_folder);
        }
    } else {
        fprintf(stderr, "Aucun dossier précédent trouvé dans '%s'.\n", dir_backup);
    }

    create_backup(source, new_backup_dir, hashes_file);
    printf("Backup initialisé avec succès dans le dossier : %s\n", new_backup_dir);
}

// Fonction pour vérifier si c'est un répertoire
int is_directory(const char *path) {
    struct stat statbuf;
    return stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode);
}

// Fonction pour copier un fichier ou un répertoire
int copy_item(const char *source, const char *destination) {
    if (is_directory(source)) {
        create_directory(destination);
        return copy_with_cp(source, destination);
    } else {
        return copy_with_cp(source, destination);
    }
}

// Fonction principale de backup
void create_backup(const char *source, const char *destination, const char *hashes) {
    if (strcmp(source, destination) == 0) {
        return;  // Ne pas faire de backup si source et destination sont les mêmes
    }

    char **files_src = list_files(source);
    char **files_dest = list_files(destination);

    if (files_src == NULL || files_dest == NULL) {
        fprintf(stderr, "Erreur lors de la récupération des fichiers.\n");
        return;
    }

    int files_src_len = array_length(files_src);
    int files_dest_len = array_length(files_dest);

    for (int i = 0; i < files_src_len; i++) {
        char src_path[MAX_PATH], dest_path[MAX_PATH];
        snprintf(src_path, MAX_PATH, "%s/%s", source, files_src[i]);
        snprintf(dest_path, MAX_PATH, "%s/%s", destination, files_src[i]);

        if (is_directory(src_path)) {
            printf("Skipping directory: %s\n", src_path);
        } else {
            deduplicate_files(src_path, dest_path, hashes);
        }
    }

    char **folders_src = list_folder(source);
    char **folders_dest = list_folder(destination);

    if (folders_src == NULL || folders_dest == NULL) {
        fprintf(stderr, "Erreur lors de la récupération des répertoires.\n");
        return;
    }

    int folders_src_len = array_length(folders_src);
    int folders_dest_len = array_length(folders_dest);

    for (int i = 0; i < folders_src_len; i++) {
        char new_src[MAX_PATH], new_dest[MAX_PATH];
        snprintf(new_src, MAX_PATH, "%s/%s", source, folders_src[i]);
        snprintf(new_dest, MAX_PATH, "%s/%s", destination, folders_src[i]);

        if (is_directory(new_src)) {
            create_directory(new_dest);
            create_backup(new_src, new_dest, hashes);
        } else {
            printf("Skipping non-directory: %s\n", new_src);
        }
    }

    // Suppression des fichiers supplémentaires dans le répertoire destination
    for (int i = 0; i < files_dest_len; i++) {
        int found = 0;
        for (int j = 0; j < files_src_len; j++) {
            if (strcmp(files_dest[i], files_src[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char dest_path[MAX_PATH];
            snprintf(dest_path, MAX_PATH, "%s/%s", destination, files_dest[i]);
            remove_directory(dest_path);
        }
    }

    // Suppression des répertoires supplémentaires dans destination
    for (int i = 0; i < folders_dest_len; i++) {
        int found = 0;
        for (int j = 0; j < folders_src_len; j++) {
            if (strcmp(folders_dest[i], folders_src[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char dest_path[MAX_PATH];
            snprintf(dest_path, MAX_PATH, "%s/%s", destination, folders_dest[i]);
            remove_directory(dest_path);
        }
    }

    // Libération de la mémoire allouée
    for (int i = 0; i < files_src_len; i++) free(files_src[i]);
    for (int i = 0; i < files_dest_len; i++) free(files_dest[i]);
    for (int i = 0; i < folders_src_len; i++) free(folders_src[i]);
    for (int i = 0; i < folders_dest_len; i++) free(folders_dest[i]);

    free(files_src);
    free(files_dest);
    free(folders_src);
    free(folders_dest);
}
