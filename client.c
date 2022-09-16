#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2022
#define BUFFER_SIZE 1024

int main(int argc, char**argv) {
 	struct sockaddr_in adresse;
 	int sockfd;

 	char buffer[BUFFER_SIZE];
 	char *adresse_serveur = "127.0.0.1";


    // Création de la socket
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);

 	if (sockfd < 0) {
  		printf("Erreur durant la création de socket\n");
  		exit(1);  
 	} else {
        printf("La socket a bien été créée\n");
    }


 	memset(&adresse, 0, sizeof(adresse));

 	adresse.sin_family = AF_INET;  
 	adresse.sin_addr.s_addr = inet_addr(adresse_serveur);
 	adresse.sin_port = PORT;     
 
 	if (connect(sockfd, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
  		printf("Erreur durant la connexion au serveur !\n");
  		exit(1);  
 	}  else {
        printf("Connecté au serveur\n");
    }

 	memset(buffer, 0, BUFFER_SIZE);
 	printf("Entrez votre message : ");

 	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
  		if (sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
   			printf("Erreur durant l'envoi des données\n\t-%s", buffer);
  		} 
  		if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL) < 0) {
   			printf("Erreur durant la réception des données\n");
  		} else {
  		 	printf("Bien reçu : ");
   			fputs(buffer, stdout);
   			printf("\n");
  		}  
 	}

 	return 0;
 }
