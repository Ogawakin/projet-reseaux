#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#define PORT 2345 
#define BUFFER_SIZE 1024

int main(int argc, char**argv) {
 	struct sockaddr_in addr;  
 	int sockfd;

 	char buffer[BUFFER_SIZE];
 	char *serverAddr = "127.0.0.1";

 	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 	if (sockfd < 0) {
  		printf("Erreur durant la création de socket\n");
  		exit(1);  
 	} else {
        printf("La socket a bien été créée\n");
    }

 	memset(&addr, 0, sizeof(addr));  
 	addr.sin_family = AF_INET;  
 	addr.sin_addr.s_addr = inet_addr(serverAddr);
 	addr.sin_port = PORT;     
 
 	if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
  		printf("Error connecting to the server!\n");  
  		exit(1);  
 	}  
 	printf("Connected to the server...\n");  

 	memset(buffer, 0, BUFFER_SIZE);
 	printf("Enter your message(s): ");

 	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
  		if (sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
   			printf("Error sending data!\n\t-%s", buffer);  
  		} 
  		if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL) < 0) {
   			printf("Error receiving data!\n");    
  		} else {
  		 	printf("Received: ");
   			fputs(buffer, stdout);
   			printf("\n");
  		}  
 	}

 	return 0;
 }
