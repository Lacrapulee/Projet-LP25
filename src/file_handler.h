#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <openssl/md5.h>

<<<<<<< Updated upstream
void list_files(const char *path);
char *read_file(const char *filepath, size_t *size);
void write_file(const char *filepath, const void *data, size_t size);

=======
// Structure pour une ligne du fichier log
typedef struct log_element {
    char *path; // Chemin du fichier/dossier
    unsigned char md5[MD5_DIGEST_LENGTH]; // MD5 du fichier dédupliqué
    char *date; // Date de dernière modification
    struct log_element *next; // Pointeur vers le prochain élément
    struct log_element *prev; // Pointeur vers l'élément précédent
} log_element;

// Structure pour une liste de log représentant le contenu du fichier backup_log
typedef struct {
    log_element *head; // Début de la liste de log 
    log_element *tail; // Fin de la liste de log
} log_t;

// Fonction permettant de lire le fichier de log .backup_log
log_t read_backup_log(const char *logfile);

// Fonction permettant de mettre à jour le fichier de log .backup_log
void update_backup_log(const char *logfile, log_t *logs);

// Fonction pour écrire un élément dans le fichier de log .backup_log
void write_log_element(log_element *elt, FILE *logfile);

// Fonction pour lister les fichiers présents dans un répertoire
void list_files(const char *path);

// Fonction pour copier un fichier d'une source vers une destination
void copy_file(const char *src, const char *dest);

>>>>>>> Stashed changes
#endif // FILE_HANDLER_H
