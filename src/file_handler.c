#include "file_handler.h"

// Fonction pour lire le contenu d'un fichier et retourner un tampon contenant les données
// `filepath` : chemin du fichier à lire
// `size` : pointeur vers une variable pour stocker la taille du fichier
void read_file(const char *filepath, size_t *size) {
    // Ouvrir le fichier en mode binaire
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        *size = 0;
        return NULL;
    }

    // Aller à la fin du fichier pour calculer sa taille
    fseek(file, 0, SEEK_END);
    *size = ftell(file); // Récupérer la position actuelle (taille totale du fichier)
    rewind(file);        // Revenir au début du fichier

    // Allouer un tampon pour contenir les données du fichier
    void *buffer = malloc(*size);
    if (buffer == NULL) {
        perror("Erreur lors de l'allocation mémoire");
        fclose(file);
        *size = 0;
        return NULL;
    }

    // Lire le contenu du fichier dans le tampon
    size_t bytesRead = fread(buffer, 1, *size, file);
    if (bytesRead != *size) {
        perror("Erreur lors de la lecture complète du fichier");
        free(buffer);
        fclose(file);
        *size = 0;
        return NULL;
    }

    // Fermer le fichier et retourner le tampon
    fclose(file);
    return buffer;
}

// Fonction pour récupérer le dernier composant (nom de fichier ou dossier) d'un chemin
// `path` : chemin complet
const char *get_last_directory(const char *path) {
    // Vérifier si le chemin est valide
    if (path == NULL || strlen(path) == 0) {
        return NULL; // Retourner NULL si le chemin est vide ou invalide
    }

    // Rechercher le dernier caractère '/' dans le chemin
    const char *last_slash = strrchr(path, '/');
    
    // Si aucun '/' n'est trouvé, le chemin correspond au fichier ou au dossier sans préfixe
    if (last_slash == NULL) {
        return path; // Retourner l'entrée complète
    }

    // Retourner la partie après le dernier '/'
    return last_slash + 1;
}

// Fonction pour lister les fichiers et répertoires d'un dossier
// `path` : chemin du répertoire à lire
// Retourne un tableau de chaînes (noms de fichiers), NULL en cas d'erreur
char **list_files(const char *path) {
    // Ouvrir le répertoire
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return NULL;
    }

    // Allouer de l'espace pour stocker les noms de fichiers (capacité initiale)
    size_t capacity = 50;
    char **tableau = malloc(capacity * sizeof(char *));
    if (tableau == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour le tableau");
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    int i = 0;

    // Lire les entrées du répertoire
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer les répertoires spéciaux "." et ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Redimensionner le tableau si nécessaire
        if (i >= capacity) {
            capacity *= 2; // Doubler la taille du tableau
            tableau = realloc(tableau, capacity * sizeof(char *));
            if (tableau == NULL) {
                perror("Erreur lors du redimensionnement du tableau");
                closedir(dir);
                return NULL;
            }
        }

        // Allouer de la mémoire pour le nom de fichier
        tableau[i] = malloc(strlen(entry->d_name) + 1);
        if (tableau[i] == NULL) {
            perror("Erreur lors de l'allocation mémoire pour un nom de fichier");
            // Libérer les ressources déjà allouées
            for (int j = 0; j < i; j++) {
                free(tableau[j]);
            }
            free(tableau);
            closedir(dir);
            return NULL;
        }

        // Copier le nom du fichier dans le tableau
        strcpy(tableau[i], entry->d_name);
        i++;
    }

    // Fermer le répertoire
    closedir(dir);

    // Marquer la fin du tableau par un pointeur NULL
    tableau[i] = NULL;

    return tableau;
}