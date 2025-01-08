#include "backup_manager.h"
#include "tools.h"

<<<<<<< Updated upstream

void create_backup(const char *source, const char *destination) {
<<<<<<< Updated upstream
    // Implémenter la logique de création d'une sauvegarde
=======
    
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

=======
// Déclarations des fonctions
void create_backup(const char *source, const char *destination, const char *old_hashes_file, const char *new_hashes_file, int verbose);
void init_backup(const char *backup_dir, const char *source, int verbose);
void restore_backup(const char *backup_id, const char *source, const char *destination, const char *hashes, int verbose);

// Initialisation du backup
void init_backup(const char *backup_dir, const char *source, int verbose) {
    if (verbose == 1) printf("[INFO] Initialisation du backup : source='%s', backup_dir='%s'\n", source, backup_dir);

    // Vérification de la validité des paramètres d'entrée
    if (source == NULL || backup_dir == NULL) {
        fprintf(stderr, "Erreur : source ou backup_dir est NULL.\n");
        return;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
        n_fichier_dest++;
=======
    // Récupère le nom du dernier répertoire de la source
    const char *last_directory = get_last_directory(source);
    if (last_directory == NULL || strlen(last_directory) == 0) {
        fprintf(stderr, "Erreur : nom de répertoire source invalide.\n");
        return;
>>>>>>> Stashed changes
    }
    
    //on compare les chunks, et on remplace...
    for (int i = 0; i < n_fichier_source; i++){
        // copy_file(source/tableau[i], destination/tableau[i]);
    } 

<<<<<<< Updated upstream
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


=======
    // Crée un chemin complet pour la destination du backup
    char source_name[MAX_PATH];
    if (snprintf(source_name, MAX_PATH, "%s/%s", backup_dir, last_directory) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin de backup trop long.\n");
        return;
    }
    create_directory(source_name);  // Création du répertoire de destination du backup

    // Recherche du dernier dossier de backup pour déterminer à partir de quel backup restaurer
    char *last_folder = find_most_recent_folder(source_name);

    // Création du répertoire pour les fichiers hash
    char hashes[MAX_PATH];
    if (snprintf(hashes, MAX_PATH, "%s/hashes", source_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin du répertoire de hashes trop long.\n");
        return;
    }
    create_directory(hashes);  // Création du répertoire "hashes"

    if (verbose == 1) printf("[INFO] Répertoire de base créé : %s\n", source_name);

    // Création du nom du backup avec un horodatage
    char timestamped_backup_name[MAX_PATH];
    get_current_time(timestamped_backup_name, sizeof(timestamped_backup_name));

    // Définition du chemin pour le nouveau répertoire de backup
    char new_backup_dir[MAX_PATH];
    if (snprintf(new_backup_dir, MAX_PATH, "%s/%s", source_name, timestamped_backup_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin de backup trop long.\n");
        return;
    }

    create_directory(new_backup_dir);  // Création du répertoire de backup avec l'horodatage
    if (verbose == 1) printf("[INFO] Nouveau répertoire de backup créé : %s\n", new_backup_dir);

    // Si un ancien dossier est trouvé, copie ce dernier dans le nouveau répertoire de backup
    if (last_folder) {
        if (copy_with_cp(last_folder, new_backup_dir) == 0) {
            if (verbose == 1) printf("[INFO] Copie réussie de '%s' vers '%s'.\n", last_folder, new_backup_dir);
        } else {
            fprintf(stderr, "Erreur lors de la copie depuis '%s'.\n", last_folder);
        }
    } else {
        fprintf(stderr, "Aucun dossier précédent trouvé dans '%s'.\n", source_name);
    }

    // Création des fichiers hash pour ce backup
    char old_hashes_file[MAX_PATH];
    char new_hashes_file[MAX_PATH];

    if (snprintf(new_hashes_file, MAX_PATH, "%s/hashes/%s.dat", source_name, timestamped_backup_name) >= MAX_PATH) {
        fprintf(stderr, "Erreur : chemin du fichier hashes trop long.\n");
        return;
    }

    if (last_folder) {
        if (snprintf(old_hashes_file, MAX_PATH, "%s/hashes/%s.dat", source_name, get_last_directory(last_folder)) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin du fichier hashes trop long.\n");
            return;
        }
    } else {
        if (snprintf(old_hashes_file, MAX_PATH, "%s/hashes/empty.dat", source_name) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin du fichier hashes trop long.\n");
            return;
        }
        FILE *old_hashFile = fopen(old_hashes_file, "a");
        if (old_hashFile) fclose(old_hashFile);  // Création d'un fichier vide pour les hash si aucun backup précédent
    }

    if (verbose == 1) printf("[INFO] Fichier 'hashes.dat' créé : %s\n", new_hashes_file);

    // Lancement de la création du backup avec les fichiers sources et destination
    create_backup(source, new_backup_dir, old_hashes_file, new_hashes_file, verbose);
}

// Fonction principale de création du backup
void create_backup(const char *source, const char *destination, const char *old_hashes_file, const char *new_hashes_file, int verbose) {
    if (verbose == 1) printf("[INFO] Création du backup : source='%s', destination='%s'\n", source, destination);

    // Vérification pour éviter de faire un backup si source et destination sont identiques
    if (strcmp(source, destination) == 0) {
        return;
    }

    // Liste les fichiers dans les répertoires source et destination
    char **source_files = list_files(source);
    char **destination_files = list_files(destination);

    if (source_files == NULL || destination_files == NULL) {
        fprintf(stderr, "Erreur lors de la récupération des fichiers.\n");
        return;
    }

    int source_files_len = array_length(source_files);
    int destination_files_len = array_length(destination_files);

    // Copie des fichiers du répertoire source vers le répertoire de destination
    for (int i = 0; i < source_files_len; i++) {
        char source_path[MAX_PATH], destination_path[MAX_PATH];
        snprintf(source_path, MAX_PATH, "%s/%s", source, source_files[i]);
        snprintf(destination_path, MAX_PATH, "%s/%s", destination, source_files[i]);

        // Si c'est un répertoire, le créer dans la destination et effectuer un backup récursif
        if (is_directory(source_path)) {
            create_directory(destination_path); 
            if (verbose == 1) printf("[INFO] Création du répertoire : %s\n", destination_path);
            create_backup(source_path, destination_path, old_hashes_file, new_hashes_file, verbose);
        } else {
            // Si c'est un fichier, faire un backup en évitant les doublons
            deduplicate_files(source_path, destination_path, old_hashes_file, new_hashes_file, verbose);
        }
    }

    // Supprime les fichiers/dossiers dans la destination qui ne sont pas dans la source
    for (int i = 0; i < destination_files_len; i++) {
        int found = 0;
        for (int j = 0; j < source_files_len; j++) {
            if (strcmp(destination_files[i], source_files[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char destination_path[MAX_PATH];
            snprintf(destination_path, MAX_PATH, "%s/%s", destination, destination_files[i]);
            remove_directory(destination_path);
            if (verbose == 1) printf("[INFO] Suppression du fichier/répertoire : %s\n", destination_path);
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    free(tableau_fichier_dest);
    free(tableau_fichier_source);
    return;
>>>>>>> Stashed changes
}

void restore_backup(const char *backup_id, const char *destination) {
    // Implémenter la logique de restauration d'une sauvegarde
}

void list_backup(const char *directory) {
    // Implémenter la logique de liste des sauvegardes
}

=======

    // Libération de la mémoire allouée pour les fichiers listés
    for (int i = 0; i < source_files_len; i++) free(source_files[i]);
    for (int i = 0; i < destination_files_len; i++) free(destination_files[i]);

    free(source_files);
    free(destination_files);
}

// Fonction de restauration du backup
void restore_backup(const char *backup_id, const char *source, const char *destination, const char *hashes, int verbose) {
    if (verbose == 1) printf("[INFO] Restauration du backup : backup_id='%s', source='%s', destination='%s'\n", backup_id, source, destination);

    // Vérifie si les chemins source et destination sont valides
    if (source == NULL || destination == NULL) {
        fprintf(stderr, "Erreur : source, destination ou backup_id est NULL.\n");
        return;
    }

    if (!is_directory(source)) {
        fprintf(stderr, "Erreur : le répertoire de sauvegarde '%s' n'existe pas.\n", source);
        return;
    }

    // Crée le répertoire de destination pour la restauration
    create_directory(destination);
    if (verbose == 1) printf("[INFO] Création du répertoire de destination : %s\n", destination);

    // Définition du chemin vers le fichier hash
    char hashes_file[MAX_PATH];
    if (hashes == NULL) {
        // Utilise un fichier hash spécifique ou crée un fichier pour la restauration
        if (snprintf(hashes_file, MAX_PATH, "%s/hashes/%s.dat", source, backup_id) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin du fichier 'hashes.dat' trop long.\n");
            return;
        }
    } else {
        if (snprintf(hashes_file, MAX_PATH, "%s", hashes) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin du fichier 'hashes.dat' trop long.\n");
            return;
        }
    }

    // Création du chemin complet vers le backup
    char backup_source_path[MAX_PATH];
    if (backup_id != NULL) {
        if (snprintf(backup_source_path, MAX_PATH, "%s/%s", source, backup_id) >= MAX_PATH) {
            fprintf(stderr, "Erreur : chemin de backup trop long.\n");
            return;
        }
        source = backup_source_path;
    }

    // Liste les fichiers dans le répertoire de backup
    char **backup_files = list_files(source);
    if (backup_files == NULL) {
        fprintf(stderr, "Erreur : impossible de lister les fichiers dans '%s'.\n", source);
        return;
    }

    int backup_files_len = array_length(backup_files);
    // Restauration des fichiers vers le répertoire de destination
    for (int i = 0; i < backup_files_len; i++) {
        char source_path[MAX_PATH], destination_path[MAX_PATH];
        snprintf(source_path, MAX_PATH, "%s/%s", source, backup_files[i]);
        snprintf(destination_path, MAX_PATH, "%s/%s", destination, backup_files[i]);

        if (is_directory(source_path)) {
            create_directory(destination_path);
            if (verbose == 1) printf("[INFO] Restauration du répertoire : %s\n", source_path);
            restore_backup(NULL, source_path, destination_path, hashes_file, verbose);
        } else {
            // Restauration du fichier en évitant les doublons
            restore_file(destination_path, source_path, hashes_file, verbose);
            if (verbose == 1) printf("[INFO] Restauration du fichier : %s\n", source_path);
        }
    }

    // Libération de la mémoire allouée pour les fichiers du backup
    for (int i = 0; i < backup_files_len; i++) free(backup_files[i]);
    free(backup_files);
}
>>>>>>> Stashed changes
