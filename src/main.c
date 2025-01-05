#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "backup_manager.h"
#include "tools.h"


// Fonction principale
int main(int argc, char *argv[]) {
    int opt;
    int verbose = 0;
    int dry_run = 0;
    char *source = NULL;
    char *destination = NULL;
    char *restore_id = NULL;
    int back_up = 0; 
    int restore = 0; 
    int list = 0; 
    char *d_server = NULL;
    char *s_server = NULL;
    int d_port = 0;
    int s_port = 0;
    
    // Définir les options long formatées et leurs raccourcis
    struct option long_options[] = {
        {"backup",       no_argument,       NULL, 'b'},
        {"restore",      required_argument, NULL, 'r'},
        {"list-backups", no_argument,       NULL, 'l'},
        {"dry-run",      no_argument,       NULL, 'd'},
        {"d-server",     required_argument, NULL, 'D'},
        {"d-port",       required_argument, NULL, 'P'},
        {"s-server",     required_argument, NULL, 'S'},
        {"s-port",       required_argument, NULL, 'p'},
        {"dest",         required_argument, NULL, 't'},
        {"source",       required_argument, NULL, 's'},
        {"verbose",      no_argument,       NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    // Analyser les arguments de la ligne de commande
    while ((opt = getopt_long(argc, argv, "brldD:P:S:p:t:s:v", long_options, NULL)) != -1) {
        switch (opt) {
            case 'b': // Sauvegarde
                if (restore != 0 || list != 0) {
                    fprintf(stderr, "Erreur : --backup ne peut pas être utilisé avec --restore ou --list-backups.\n");
                    return EXIT_FAILURE;
                }else {
                    back_up = 1; 
                }
                break;
            case 'r': // Restauration
                if (back_up != 0 || list != 0) {
                    fprintf(stderr, "Erreur : --restore ne peut pas être utilisé avec --backup ou --list-backups.\n");
                    return EXIT_FAILURE;
                }else {
                    restore_id = optarg;
                    restore = 1;
                }
                break;
            case 'l': // Liste des sauvegardes
                if (back_up != 0 || restore != 0) {
                    fprintf(stderr, "Erreur : --list-backups ne peut pas être utilisé avec --backup ou --restore.\n");
                    return EXIT_FAILURE;
                }else {
                    list = 1; 
                }
                break;
            case 'd': // Dry-run
                dry_run = 1;
                break;
            case 'D': // Serveur de destination
                d_server = optarg;
                break;
            case 'P': // Port du serveur de destination
                d_port = atoi(optarg);
                break;
            case 'S': // Serveur source
                s_server = optarg;
                break;
            case 'p': // Port du serveur source
                s_port = atoi(optarg);
                break;
            case 't': // Chemin de destination
                destination = optarg;
                break;
            case 's': // Chemin source
                source = optarg;
                break;
            case 'v': // Afficher des informations détaillées
                verbose = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s --backup | --restore | --list-backups [options]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (verbose) {
        printf("Mode verbeux activé\n");
    }
    //Lister toutes les sauvegardes.
    if (list){
        list_backups(destination, verbose, 0);
    }
    //Vérifier que l'utilisateur a fourni les informations nécessaires
    if ((source == NULL && destination == NULL) && (back_up != 0 || restore != 0)) {
        fprintf(stderr, "Erreur : Source et destination doivent être spécifiées.\n");
        return EXIT_FAILURE;
    }

    // Gestion des options
    if (optind < argc) {
        // Si on a encore des arguments après l'analyse, cela signifie une erreur
        fprintf(stderr, "Usage: %s --backup | --restore | --list-backups [options]\n", argv[0]);
        return EXIT_FAILURE;
    }


    if (dry_run) {
        handle_dry_run(source, destination, restore_id, back_up);
    }

    if (d_server != NULL || s_server != NULL) {
        // On peut aussi ajouter des vérifications pour les serveurs ici
        if (verbose) {
            printf("Serveur de destination : %s, Port : %d\n", d_server, d_port);
            printf("Serveur source : %s, Port : %d\n", s_server, s_port);
        }
    }
    
    // Commandes en fonction des options
    if (restore) {
        if (verbose) printf("Restauration à partir de : %s\n", restore_id);
        restore_backup(restore_id, source, destination, NULL, verbose);
    }

    if (back_up) {
        if (verbose) printf("Création de la sauvegarde depuis : %s vers : %s\n", source, destination);
        init_backup(destination, source, verbose);
    }
    return EXIT_SUCCESS;
}