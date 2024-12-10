#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

/*
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

//affiche la liste 
void affiche_liste(liste *l){

    liste *suiv = l->suivant;
    printf("Valeur de la liste : \n");

    while(suiv != NULL){
        printf("Valeur int : %d, Valeur char : %s\n", suiv->valeur_int, suiv->valeur_str);
        suiv = suiv->suivant;
    }

    return;
} */

