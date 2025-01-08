#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"


//Fichier TOOLS utilisé par les autres scripts; 
// Définition de la fonction init_liste
liste *init_liste(int valeur_int, const char *valeur_str) {
    // Allocation mémoire pour un nouvel élément de la liste
    liste *l = malloc(sizeof(liste));
    if (l == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des valeurs
    l->valeur_int = valeur_int;

    if (valeur_str != NULL) {
        l->valeur_str = malloc(strlen(valeur_str) + 1); // Allocation pour la chaîne
        if (l->valeur_str == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire échouée pour la chaîne.\n");
            free(l);
            exit(EXIT_FAILURE);
        }
        strcpy(l->valeur_str, valeur_str); // Copie de la chaîne
    } else {
        l->valeur_str = NULL; // Si aucune chaîne n'est donnée
    }

    l->suivant = NULL; // Aucun suivant pour l'instant

    return l;
}

//Ajoute à la fin d'une liste 
liste *ajou_fin_liste(liste *l, int valeur_int, char *valeur_str) {
    // Création de l'élément à aujouter 
    liste *nouv = init_liste(valeur_int, valeur_str);
    liste    *prec = NULL;
    liste    *suiv = l;

    if (l == NULL) {
        return nouv;
    }

    while (suiv != NULL) {
        prec = suiv;
        suiv = suiv->suivant;
    };
    
    prec->suivant = nouv;

    return l;
}

//Ajoute au début de la liste
liste *ajoute_debut(liste *l, int valeur_int, const char *valeur_str){
    // Création de l'élément à aujouter 
    liste *nouv = init_liste(valeur_int, valeur_str);
    if (l == NULL) {
        return nouv;
    }else{
    nouv->suivant = l;
    return nouv;
    }
}

<<<<<<< Updated upstream
//affiche la liste 
void affiche_liste(liste *l){
=======
// Fonction pour créer un répertoire si il n'existe pas
// `path` : chemin du répertoire à créer
void create_directory(const char *path) {
    struct stat st = {0};
    // Vérifier si le répertoire existe déjà
    if (stat(path, &st) == -1) {
        // Si le répertoire n'existe pas, on le crée avec les permissions par défaut (0755)
        if (mkdir(path, 0777) == -1) {
            perror("Erreur lors de la création du répertoire");
        }
    }
}
>>>>>>> Stashed changes

    liste *suiv = l->suivant;
    printf("Valeur de la liste : \n");

    while(suiv != NULL){
        printf("Valeur int : %d, Valeur char : %s\n", suiv->valeur_int, suiv->valeur_str);
        suiv = suiv->suivant;
    }

    return;
}


<<<<<<< Updated upstream
=======
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

// Fonction pour lister les sauvegardes dans un répertoire local
// `dir_backup` : chemin du répertoire des sauvegardes
void list_backups(const char *dir_backup, int verbose, int level) {
    if (dir_backup != NULL) {
        // Affichage du répertoire de sauvegarde seulement si level == 0
        if (level == 0) {
            printf("Listing des sauvegardes dans : \n%s\n", dir_backup);
        }

        // Logique pour lister les fichiers de sauvegarde dans le répertoire spécifié
        char **liste = list_files(dir_backup); // Récupérer la liste des fichiers/dossiers

        // Pour chaque élément dans la liste, afficher avec un certain nombre de tirets
        for (int j = 0; liste[j] != NULL; j++) {
            // Afficher les tirets en fonction du niveau d'imbrication (level)
            for (int k = 0; k < level; k++) {
                printf("-"); // Affichage des tirets pour l'indentation
            }
            printf(" %s\n", liste[j]); // Afficher le nom du fichier/dossier

            // Si la verbosité est activée et si c'est un répertoire, appeler récursivement la fonction
            if (verbose) {
                char path[MAX_PATH];
                snprintf(path, MAX_PATH, "%s/%s", dir_backup, liste[j]);
                if (is_directory(path)) {
                    list_backups(path, verbose, level + 1); // Appel récursif pour les sous-dossiers
                }
            }
        }

        // Libération de la mémoire allouée pour la liste des fichiers/dossiers
        for (int i = 0; liste[i] != NULL; i++) {
            free(liste[i]);
        }
        free(liste);
    }
}
>>>>>>> Stashed changes
