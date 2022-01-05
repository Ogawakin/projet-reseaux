#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#define PORT 2345
#define BUF_SIZE 1024
#define CLADDR_LEN 100

int main() {
	struct sockaddr_in serv_addr, cl_addr;
	socklen_t addr_len;
 	int sockfd, newsockfd;
 	char buffer[BUF_SIZE];
 	char clientAddr[CLADDR_LEN];
 
 	//Création de la socket
 	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
 	{
 		printf("Error creating socket!\n");
  		exit(1);
 	}
 	printf("Socket created...\n");
 
 	//Création de l'adresse du serveur
 	memset(&serv_addr, 0, sizeof(serv_addr));
 	serv_addr.sin_family = AF_INET;
 	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 	serv_addr.sin_port = htons(PORT); 
 
 	//Connexion/Liaison
 	if (bind(sockfd, (struct sockaddr *) &serv_addr, (socklen_t *) sizeof(serv_addr)) < 0)
 	{
 		printf("Error binding!\n");
  		exit(1);
 	}
 	printf("Binding done...\n");

    char *ip = inet_ntoa(serv_addr.sin_addr);
    printf("%s\n", ip);
	//Mise en place de l'écoute du serveur
 	listen(sockfd, 5); //Cinq joueurs max
 	printf("Listening...\nWaiting for a connection...\n");

	//Tant que le serveur est en écoute:
 	while(1) 
 	{ 
  		//TODO:mettre newsockfd dans la liste des clients 
  		addr_len = sizeof(cl_addr);
  		//On accepte la connexion d'un joueur 
  		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &addr_len)) < 0) 
  		{
   			printf("Error accepting connection!\n");
   			exit(1);
  		}
  		printf("Connection accepted...\n");
 	}
		while(1) 
		{
			memset(buffer, 0, BUF_SIZE);
			//Le serveur reçoit un message d'un client
			if(recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &addr_len) < 0) 
			{
				printf("Error receiving data!\n");  
 				exit(1);
			}
			printf("Received data from %s: %s\n", clientAddr, buffer);

			//Il retourne le message reçu à l'envoyeur   
			if (sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, addr_len) < 0) 
			{  
 				printf("Error sending data!\n");  
 				exit(1);  
			}  
			printf("Sent data to %s: %s\n", clientAddr, buffer);
		}

 	return 0;
}
