#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#define PORT 2345
#define BUFFER_SIZE 1024
#define CLADDR_LEN 100

int main() {
	struct sockaddr_in addr, cl_addr;
	socklen_t len;
 	int sockfd, newsockfd;
 	char buffer[BUFFER_SIZE];

 	//Création de la socket
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
 	if (sockfd < 0)	{
 		printf("Error creating socket!\n");
  		exit(1);
 	}
 	printf("Socket created...\n");

 	//Création de l'adresse du serveur
 	memset(&addr, 0, sizeof(addr));
 	addr.sin_family = AF_INET;
 	addr.sin_addr.s_addr = INADDR_ANY;
 	addr.sin_port = PORT; //htons?

 	//Connexion/Liaison
 	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
 		printf("Error binding!\n");
  		exit(1);
 	}
 	printf("Binding done...\n");

	//Mise en place de l'écoute du serveur
 	printf("Waiting for a connection...\n");
 	listen(sockfd, 5); //Cinq joueurs max

	//Tant que le serveur est en écoute:
 	while(1) {
  		len = sizeof(cl_addr);

  		//On accepte la connexion d'un joueur 
  		newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  		if (newsockfd < 0) {
   			printf("Error accepting connection!\n");
   			exit(1);
  		}

  		printf("Connection accepted...\n");

		while(1) {
			memset(buffer, 0, BUFFER_SIZE);

			//Le serveur reçoit un message d'un client
			if(recvfrom(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cl_addr, &len) < 0) {
				printf("Error receiving data!\n");  
 				exit(1);
			}

			printf("Received data : %s\n", buffer);

			//Il retourne le message reçu à l'envoyeur   
			if (sendto(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cl_addr, len) < 0) {
 				printf("Error sending data!\n");  
 				exit(1);  
			}  
			printf("Sent data : %s\n", buffer);
		}
 	}
 	return 0;
 }
