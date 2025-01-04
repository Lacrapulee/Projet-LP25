#include <stdio.h>
#include <stdlib.h>
#include "network.h"

int main() {
    const char *data = "Bonjour";
    const char *server_address = "127.0.0.1";
    int port = 8080;

    // Démarrer un thread ou un processus pour le serveur ici si nécessaire

    // Test de l'envoi de données
    printf("Envoi des données au serveur...\n");
    send_data(server_address, port, data, strlen(data));

    // Test de réception (le serveur doit être prêt à recevoir)
    printf("Réception des données depuis un client...\n");
    char *received_data = receive_data(port, NULL);
    if (received_data) {
        printf("Données reçues : %s\n", received_data);
        free(received_data);
    }

    return 0;
}
