#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/select.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/wait.h>
#include <sys/time.h>

#define PORT 2022
#define BUFFER_SIZE 1024
#define CLADDR_LEN 100

int main() {
	struct sockaddr_in adresse, adresse_client;
	socklen_t len;
 	int sockfd, newsockfd;
 	char buffer[BUFFER_SIZE];


 	// Création de la socket
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("Erreur durant la création de socket\n");
        exit(1);
    } else {
        printf("La socket a bien été créée\n");
    }


 	// Création de l'adresse du serveur
 	memset(&adresse, 0, sizeof(adresse));

 	adresse.sin_family = AF_INET;
 	adresse.sin_addr.s_addr = INADDR_ANY;
 	adresse.sin_port = PORT;

 	// Connexion/Liaison
 	if (bind(sockfd, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
 		printf("Erreur pendant la liaison\n");
  		exit(1);
 	} else {
        printf("Liaison effectuée avec succès\n");
 	}

	// Mise en place de l'écoute du serveur
 	printf("En attente d'une connexion.\n");

    // Deux joueurs maximum
 	listen(sockfd, 2);

	// Tant que le serveur est en écoute:
 	while (1) {
  		len = sizeof(adresse_client);

  		//On accepte la connexion d'un joueur 
  		newsockfd = accept(sockfd, (struct sockaddr *) &adresse_client, &len);

  		if (newsockfd < 0) {
   			printf("Erreur durant l'acceptation de la connexion\n");
   			exit(1);
  		} else {
            printf("Connexion acceptée avec succès\n");
  		}

		while (1) {
			memset(buffer, 0, BUFFER_SIZE);

			// Le serveur reçoit un message d'un client
			if(recvfrom(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &adresse_client, &len) < 0) {
				printf("Erreur durant la réception des données\n");
 				exit(1);
			} else {
                printf("Données reçues: %s\n", buffer);
			}


			// et il renvoie le message à l'envoyeur
			if (sendto(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &adresse_client, len) < 0) {
 				printf("Erreur dans l'envoi des données\n");
 				exit(1);  
			} else {
                printf("Données envoyées : %s\n", buffer);
			}
		}
 	}

 	return 0;
 }
