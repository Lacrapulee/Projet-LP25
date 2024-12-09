#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "tools.h"



int main(int argc, char *argv[]) {
    // Analyse des arguments de la ligne de commande

    // Implémentation de la logique de sauvegarde et restauration
    // Exemples : gestion des options --backup, --restore, etc.
    char **tableau = malloc(50 * sizeof(char *));

    tableau = list_files("/home/gauthier/Documents/GitHub/Projet-LP25/src");

    for (int i = 0; i < 16; i++){
        printf("%s \n", tableau[i]); 
    }
    return EXIT_SUCCESS;
}

