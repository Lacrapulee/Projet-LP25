#include "backup_manager.h"

// Déclarations des fonctions
void create_backup(const char *source, const char *destination, const char *hashes, int v);
void init_backup(const char *dir_backup, const char *source, int v);
void restore_backup(const char *backup_id, const char *source, const char *dest, const char *hashes, int v);

// Initialisation du backup
void init_backup(const char *dir_backup, const char *source, int v) {
    if (v == 1) printf("[INFO] Initialisation du backup : source='%s', dir_backup='%s'\n", source, dir_backup);

    // Vérifie si les chemins source et dir_backup sont valides
    if (source == NULL || dir_backup == NULL) {
        fprintf(stderr, "Erreur : source ou dir_backup est NULL.\n");
        return;
    }

    // Obtient le dernier nom de répertoire du chemin source
    const char *last_name = get_last_directory(source);
    if (last_name == NULL || strlen(last_name) == 0) {
        fprintf(stderr, "Erreur : nom de fichier source invalide.\n");
        return;
    }

    char src_name[MAX_PATH];
    // Crée un chemin pour le répertoire de sauvegarde
    if (snprintf(src_name, MAX_PATH, "%s/%s", dir_backup, last_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin de backup trop long.\n");
        return;
    }

    create_directory(src_name);
    if (v == 1) printf("[INFO] Répertoire de base créé : %s\n", src_name);

    // Recherche du dernier dossier créé
    char *last_folder = find_most_recent_folder(src_name);
    char timestamped_backup_name[MAX_PATH];
    // Obtient un nom de répertoire avec un timestamp
    get_current_time(timestamped_backup_name, sizeof(timestamped_backup_name));

    char new_backup_dir[MAX_PATH];
    // Crée un nouveau chemin pour le répertoire de sauvegarde avec le timestamp
    if (snprintf(new_backup_dir, MAX_PATH, "%s/%s", src_name, timestamped_backup_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin de backup trop long.\n");
        return;
    }
    create_directory(new_backup_dir);
    if (v == 1) printf("[INFO] Nouveau répertoire de backup créé : %s\n", new_backup_dir);

    // Crée un fichier pour stocker les hashes des fichiers sauvegardés
    char hashes_file[MAX_PATH];
    if (snprintf(hashes_file, MAX_PATH, "%s/hashes.dat", src_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin du fichier hashes trop long.\n");
        return;
    }

    FILE *hash_file = fopen(hashes_file, "a");
    if (hash_file == NULL) {
        perror("Erreur lors de la création du fichier 'hashes.dat'");
        return;
    }
    fclose(hash_file);
    if (v == 1) printf("[INFO] Fichier 'hashes.dat' créé : %s\n", hashes_file);

    // Copie les fichiers du dernier dossier trouvé
    if (last_folder) {
        if (copy_with_cp(last_folder, new_backup_dir) == 0) {
            if (v == 1) printf("[INFO] Copie réussie de '%s' vers '%s'.\n", last_folder, new_backup_dir);
        } else {
            fprintf(stderr, "Erreur lors de la copie depuis '%s'.\n", last_folder);
        }
    } else {
        fprintf(stderr, "Aucun dossier précédent trouvé dans '%s'.\n", src_name);
    }

    // Crée un backup à partir du répertoire source vers le nouveau répertoire
    create_backup(source, new_backup_dir, hashes_file, v);
}

// Fonction principale de backup
void create_backup(const char *source, const char *destination, const char *hashes, int v) {
    if (v == 1) printf("[INFO] Création du backup : source='%s', destination='%s'\n", source, destination);

    // Evite un backup si source et destination sont les mêmes
    if (strcmp(source, destination) == 0) {
        return;
    }

    // Liste les fichiers présents dans le répertoire source et destination
    char **files_src = list_files(source);
    char **files_dest = list_files(destination);

    if (files_src == NULL || files_dest == NULL) {
        fprintf(stderr, "Erreur lors de la récupération des fichiers.\n");
        return;
    }

    int files_src_len = array_length(files_src);
    int files_dest_len = array_length(files_dest);

    // Sauvegarde les fichiers source dans la destination
    for (int i = 0; i < files_src_len; i++) {
        char src_path[MAX_PATH], dest_path[MAX_PATH];
        snprintf(src_path, MAX_PATH, "%s/%s", source, files_src[i]);
        snprintf(dest_path, MAX_PATH, "%s/%s", destination, files_src[i]);

        // Si c'est un répertoire, créer un répertoire dans la destination et y faire un backup
        if (is_directory(src_path)) {
            create_directory(dest_path); 
            if (v == 1) printf("[INFO] Création du répertoire : %s\n", dest_path);
            create_backup(src_path, dest_path, hashes, v);
        } else {
            // Si c'est un fichier, faire un backup et éviter les doublons
            deduplicate_files(src_path, dest_path, hashes, v);
        }
    }

    // Supprimer les fichiers/dossiers dans la destination qui ne sont pas présents dans la source
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
            if (v == 1) printf("[INFO] Suppression du fichier/répertoire : %s\n", dest_path);
        }
    }

    // Libération de la mémoire allouée pour les fichiers
    for (int i = 0; i < files_src_len; i++) free(files_src[i]);
    for (int i = 0; i < files_dest_len; i++) free(files_dest[i]);

    free(files_src);
    free(files_dest);
}

// Fonction de restauration
void restore_backup(const char *backup_id, const char *source, const char *dest, const char *hashes, int v) {
    if (v == 1) printf("[INFO] Restauration du backup : backup_id='%s', source='%s', dest='%s'\n", backup_id, source, dest);

    // Vérifie si les chemins source et destination sont valides
    if (source == NULL || dest == NULL) {
        fprintf(stderr, "Erreur : source, destination ou backup_id est NULL.\n");
        return;
    }

    if (!is_directory(source)) {
        fprintf(stderr, "Erreur : le répertoire de sauvegarde '%s' n'existe pas.\n", source);
        return;
    }

    // Crée le répertoire de destination pour la restauration
    create_directory(dest);
    if (v == 1) printf("[INFO] Création du répertoire de destination : %s\n", dest);

    // Utilisation d'un fichier hashes spécifique ou création de l'un nouveau pour la restauration
    char hashes_name[MAX_PATH];
    if (hashes == NULL) {
        if (snprintf(hashes_name, MAX_PATH, "%s/hashes_restore.dat", source) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin du fichier 'hashes.dat' trop long.\n");
            return;
        }
        hashes = hashes_name;
    }

    // Création du chemin complet vers le backup spécifié
    char src_backup_path[MAX_PATH];
    if (backup_id != NULL) {
        if (snprintf(src_backup_path, MAX_PATH, "%s/%s", source, backup_id) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin de backup trop long.\n");
            return;
        }
        source = src_backup_path;
    }

    // Liste les fichiers dans le répertoire de backup
    char **files_backup = list_files(source);
    if (files_backup == NULL) {
        fprintf(stderr, "Erreur : impossible de lister les fichiers dans '%s'.\n", source);
        return;
    }

    int files_backup_len = array_length(files_backup);
    // Restaure les fichiers dans le répertoire de destination
    for (int i = 0; i < files_backup_len; i++) {
        char src_path[MAX_PATH], dest_path[MAX_PATH];
        snprintf(src_path, MAX_PATH, "%s/%s", source, files_backup[i]);
        snprintf(dest_path, MAX_PATH, "%s/%s", dest, files_backup[i]);

        if (is_directory(src_path)) {
            create_directory(dest_path);
            if (v == 1) printf("[INFO] Restauration du répertoire : %s\n", src_path);
            restore_backup(NULL, src_path, dest_path, hashes, v);
        } else {
            // Restauration du fichier en évitant les doublons
            deduplicate_files(src_path, dest_path, hashes, v);
            if (v == 1) printf("[INFO] Restauration du fichier : %s\n", src_path);
        }
    }

    // Libération de la mémoire allouée pour les fichiers
    for (int i = 0; i < files_backup_len; i++) free(files_backup[i]);
    free(files_backup);
}