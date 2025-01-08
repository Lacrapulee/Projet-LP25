#ifndef TOOLS
#define TOOLS
<<<<<<< Updated upstream

=======
#define _GNU_SOURCE  // Pour activer asprintf
#include "file_handler.h"
#define MAX_PATH 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ftw.h>  // Pour suppression récursive des répertoires

void create_or_clear_file(const char *filepath);
int is_directory(const char *path); 
int copy_item(const char *source, const char *destination); 
void create_directory(const char *path); 
void get_current_time(char *buffer, size_t size); 
char *find_most_recent_folder(const char *directory); 
int array_length(char **array); 
int copy_with_cp(const char *source, const char *destination); 
void remove_directory(const char *path);
void list_backups(const char *dir_backup, int verbose, int i); 
void handle_dry_run(const char *source, const char *destination, const char *backup_id, int back_up);
>>>>>>> Stashed changes

// Définition de la structure liste
typedef struct liste {
    int valeur_int;           // Valeur entière
    char *valeur_str;         // Pointeur pour une chaîne de caractères
    struct liste *suivant;    // Pointeur vers l'élément suivant de la liste
} liste;

// Déclaration des fonctions
liste *init_liste(int valeur_int, const char *valeur_str); //Initialisation 
liste *ajou_fin_liste(liste *l, int valeur_int, char *valeur_str); // Ajoute un élément à la fin de la liste
liste *ajoute_debut(liste *l, int valeur_int, const char *valeur_str); // Ajoute un élément au début de la liste
void affiche_liste(liste *l); //Trivial nan ? 

#endif // TOOLS
