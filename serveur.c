#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

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

 	//Connexion/Liaison
 	if (bind(sockfd, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
 		printf("Error binding!\n");
  		exit(1);
 	}
 	printf("Binding done...\n");

	//Mise en place de l'écoute du serveur
 	printf("Waiting for a connection...\n");
 	listen(sockfd, 5); //Cinq joueurs max

	//Tant que le serveur est en écoute:
 	while(1) {
  		len = sizeof(adresse_client);

  		//On accepte la connexion d'un joueur 
  		newsockfd = accept(sockfd, (struct sockaddr *) &adresse_client, &len);
  		if (newsockfd < 0) {
   			printf("Error accepting connection!\n");
   			exit(1);
  		}

  		printf("Connection accepted...\n");

		while(1) {
			memset(buffer, 0, BUFFER_SIZE);

			//Le serveur reçoit un message d'un client
			if(recvfrom(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &adresse_client, &len) < 0) {
				printf("Error receiving data!\n");  
 				exit(1);
			}

			printf("Received data : %s\n", buffer);

			//Il retourne le message reçu à l'envoyeur   
			if (sendto(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &adresse_client, len) < 0) {
 				printf("Error sending data!\n");  
 				exit(1);  
			}  
			printf("Sent data : %s\n", buffer);
		}
 	}
 	return 0;
 }
