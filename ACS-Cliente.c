/*Proyecto Final de la Materia de Arquitectura Cliente-Servidor
Objetivo: Crear un Cliente-Servidor que ejecute comandos
remotamente como ocurre con un cliente-Servidor SSH comercial 
Cortes Lopez Maricela
Hernandez Calderon Fernando*/

//Bibliotecas necesarias.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>

// Puerto por el que se conectara el cliente
#define PORT 3490
// Numero maximo de bytes en el mensaje
#define MAXDATASIZE 300

int main(int argc, char *argv[]){
  int sockfd, numbytes;
  // Inicializamos el buffer con el tamaño maximo de bytes
  char buf[MAXDATASIZE];
  //Estructura para almacenar la informacion del host
  struct hostent *he;

  // Informacion de los conectores
  struct sockaddr_in their_addr;
  
  // Si no se ingresa un segundo argumento
  if(argc != 2){
    fprintf(stderr, "Client-Usage: %s host_servidor\n", argv[0]);
    exit(1);
  }

  // Obtenemos la informacion del host
  if((he=gethostbyname(argv[1])) == NULL){
    perror("gethostbyname()");
    exit(1);
  }
  else
    printf("Client-The remote host is: %s\n", argv[1]);

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket()");
    exit(1);
  }
  else 
    printf("Client-The socket() sockfd is OK...\n");

  their_addr.sin_family = AF_INET;
  printf("Server-Using %s and port %d...\n", argv[1], PORT);
  their_addr.sin_port = htons(PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);

  memset(&(their_addr.sin_zero), '\0', 8);
  if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect()");
    exit(1);
  }
  else
    printf("Client-The connect() is OK...\n");

  if(send(sockfd, "Paterno Materno Nombre\n", 29, 0) == -1)
    perror("Server-send() error lol!");
  else
    printf("Server-send is OK...!\n");


  printf("Client-Closing sockfd\n");
  close(sockfd);
  return 0;
}
