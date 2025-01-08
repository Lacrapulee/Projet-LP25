#include "tools.h"

// Fonction pour vérifier si le chemin est un répertoire
// `path` : chemin à vérifier
// Retourne 1 si c'est un répertoire, 0 sinon
int is_directory(const char *path) {
    struct stat statbuf;
    return stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode);
}

// Fonction pour copier un fichier ou un répertoire
// `source` : chemin de la source
// `destination` : chemin de la destination
// Si la source est un répertoire, créer et copier récursivement
// Retourne 0 si tout se passe bien, -1 en cas d'erreur
int copy_item(const char *source, const char *destination) {
    if (is_directory(source)) {
        create_directory(destination); // Créer le répertoire si nécessaire
        return copy_with_cp(source, destination); // Copier le répertoire avec la commande cp
    } else {
        return copy_with_cp(source, destination); // Copier un fichier avec la commande cp
    }
}

// Fonction pour créer un répertoire si il n'existe pas
// `path` : chemin du répertoire à créer
void create_directory(const char *path) {
    struct stat st = {0};
    // Vérifier si le répertoire existe déjà
    if (stat(path, &st) == -1) {
        // Si le répertoire n'existe pas, on le crée avec les permissions par défaut (0755)
        if (mkdir(path, 0755) == -1) {
            perror("Erreur lors de la création du répertoire");
        }
    }
}

// Fonction pour récupérer l'heure actuelle formatée avec gettimeofday
// `buffer` : tampon pour stocker l'heure formatée
// `size` : taille maximale du tampon
void get_current_time(char *buffer, size_t size) {
    struct timeval tv;
    gettimeofday(&tv, NULL); // Obtenir l'heure actuelle avec une précision en microsecondes
    struct tm *t = localtime(&tv.tv_sec); // Convertir l'heure en structure tm

    // Formater l'heure avec les secondes et microsecondes
    strftime(buffer, size, "%Y.%m.%d.%H.%M.%S", t);
    // Ajouter les microsecondes à la fin de l'heure formatée
    snprintf(buffer + strlen(buffer), size - strlen(buffer), ".%06ld", tv.tv_usec);
}

// Fonction pour trouver le dossier le plus récent dans un répertoire
// `directory` : chemin du répertoire à scanner
// Retourne le chemin du dossier le plus récent, ou NULL si aucun dossier n'est trouvé
char *find_most_recent_folder(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    time_t latest_time = 0;
    static char latest_folder[MAX_PATH] = "";

    // Ouvrir le répertoire
    if ((dir = opendir(directory)) == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return NULL;
    }

    // Parcourir toutes les entrées du répertoire
    while ((entry = readdir(dir)) != NULL) {
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name); // Construire le chemin complet

        // Vérifier si l'entrée est un répertoire
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // Ignorer les répertoires spéciaux "." et ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            // Si le dossier est plus récent, mettre à jour la variable latest_folder
            if (statbuf.st_mtime > latest_time) {
                latest_time = statbuf.st_mtime;
                strncpy(latest_folder, path, sizeof(latest_folder) - 1); // Sauvegarder le chemin du dossier le plus récent
            }
        }
    }

    // Fermer le répertoire
    closedir(dir);

    // Retourner le dossier le plus récent, ou NULL si aucun dossier n'a été trouvé
    return latest_folder[0] != '\0' ? latest_folder : NULL;
}

// Fonction pour compter le nombre d'éléments dans un tableau dynamique de chaînes
// `array` : tableau de chaînes
// Retourne le nombre d'éléments dans le tableau
int array_length(char **array) {
    int count = 0;
    while (array[count] != NULL) {
        count++; // Compter les éléments jusqu'à ce que l'élément NULL soit atteint
    }
    return count;
}

// Fonction pour copier un fichier ou répertoire avec la commande 'cp'
// `source` : chemin de la source
// `destination` : chemin de la destination
// Retourne 0 en cas de succès, -1 en cas d'erreur
int copy_with_cp(const char *source, const char *destination) {
    char command[4096];
    // Construire la commande cp pour copier récursivement
    snprintf(command, sizeof(command), "cp -r '%s'/* '%s'", source, destination);

    // Exécuter la commande
    int result = system(command);

    // Vérifier le résultat de la commande
    if (result != 0) {
        fprintf(stderr, "Erreur : impossible de copier '%s' vers '%s' avec cp.\n", source, destination);
        return -1;
    }

    return 0;
}

// Fonction de rappel pour supprimer un fichier ou répertoire récursivement
// `fpath` : chemin du fichier ou répertoire à supprimer
// `sb` : structure de statut du fichier
// `typeflag` : type de fichier (répertoire ou fichier normal)
// `ftwbuf` : structure de contrôle de l'itérateur
// Retourne 0 si la suppression réussit
int remove_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    if (typeflag == FTW_D) {
        return rmdir(fpath);  // Supprimer les répertoires vides
    }
    return unlink(fpath);  // Supprimer les fichiers avec unlink()
}

// Fonction pour supprimer un répertoire et son contenu de manière récursive
// `path` : chemin du répertoire à supprimer
void remove_directory(const char *path) {
    nftw(path, remove_callback, 64, FTW_DEPTH | FTW_PHYS); // Utiliser nftw pour supprimer récursivement
}

// Fonction pour lister les sauvegardes dans un répertoire local
// `dir_backup` : chemin du répertoire des sauvegardes
void list_backups(const char *dir_backup, int verbose, int i) {

    if (dir_backup != NULL) {
        if (i==0)printf("Listing des sauvegardes dans '%s'.\n", dir_backup);
        // Logique pour lister les sauvegardes dans le répertoire spécifié
        for(int y = 0; y < i; y++ ){
            printf("-");
        }
        char **liste = list_files(dir_backup);
        for (int j = 0; liste[j] != NULL; j++) {
            printf("- %s \n", liste[j]); // Afficher chaque sauvegarde
            if(verbose){
                char path[MAX_PATH];
                snprintf(path, MAX_PATH, "%s/%s", dir_backup, liste[j]);
                if(is_directory(path))list_backups(path, verbose, i+1);
            }
        }
    }

    // Logique pour lister les sauvegardes sur un serveur distant ici...
}

// Fonction de gestion de l'option --dry-run (simulation)
// `source` : chemin de la source
// `destination` : chemin de la destination
// `backup_id` : identifiant de la sauvegarde
// `back_up` : indique s'il s'agit d'une sauvegarde ou d'une restauration
void handle_dry_run(const char *source, const char *destination, const char *backup_id, int back_up) {
    printf("Mode Dry-run activé. Aucune copie réelle n'est effectuée.\n");

    // Afficher un message selon qu'il s'agit d'une sauvegarde ou d'une restauration
    if (back_up) {
        printf("Sauvegarde simulée de '%s' vers '%s'.\n", source, destination);
    } else {
        printf("Restauration simulée depuis '%s' vers '%s' de la version : %s.\n", source, destination, backup_id);
    }
}

