/*Proyecto Final de la Materia de Arquitectura Cliente-Servidor
Objetivo: Crear un Cliente-Servidor que ejecute comandos
remotamente como ocurre con un cliente-Servidor SSH comercial 
Cortes Lopez Maricela
Hernandez Calderon Fernando
                                                      Archivo Cliente*/

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
  char bufEnvia[MAXDATASIZE];
  char bufRecibe[MAXDATASIZE];
  //Estructura para almacenar la informacion del host
  struct hostent *he;

  // Informacion de los conectores
  struct sockaddr_in their_addr;
  
  // Si no se ingresa un segundo argumento
  if(argc != 2){
    fprintf(stderr, "Client-Usa: %s host_servidor\n", argv[0]);
    exit(1);
  }

  // Intentamos obtener la informacion del host
  if((he=gethostbyname(argv[1])) == NULL){
    perror("gethostbyname()");
    exit(1);
  }
  else
    printf("Client-El host remoto es: %s\n", argv[1]);

  // Intentamos abrir el socket para iniciar la comunicacion
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket()");
    exit(1);
  } else { 
    printf("Client-Se logro crear socket...\n");
  }

  // Configuramos los detalles de la conexion
  their_addr.sin_family = AF_INET;
  printf("Server-Using %s and port %d...\n", argv[1], PORT);
  their_addr.sin_port = htons(PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);

  //Ponemos los demas parametros de la estructura en 0
  memset(&(their_addr.sin_zero), '\0', 8);

  //Probamos si podemos conectarnos al servidor
  if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect()");
    exit(1);
  } else{
    printf("Client-Conectado...\n");
  }
  
  // Comenzamos a comunicarnos con el servidor
  while (strcmp(bufEnvia,"exit\n") != 0) {
    //Limpiamos el vector bufEnvia y bufRecibe
    memset(bufEnvia,0,MAXDATASIZE);
    memset(bufRecibe,0,MAXDATASIZE);

    // Escribimos el comando a ejecutar
  	printf("client> ");
  	fgets(bufEnvia,MAXDATASIZE,stdin);

    //Intentamos enviar el mensaje al servidor
  	if(send(sockfd, bufEnvia, 29, 0) == -1){
    	perror("Server-send() error lol!");
  	} else{
    	//printf("Servidor- Recibio %s\n", bufEnvia);
	  }

    //Intentamos recibir la respuesta del servidor
    //if((numbytes = recv(sockfd, bufRecibe, MAXDATASIZE-1, 0)) == -1){
    //  perror("recv()");
    //  exit(1);
    //}else {
      //Ponemos fin de cadena al mensaje
    //  bufRecibe[numbytes] = '\0';
    //  printf("Servidor- Envia %s", bufRecibe);  
    //}
    
    fflush(stdout);
    fflush(stdin);
    while (strcmp(bufRecibe,"termine\n") != 0){
          if (strlen(bufRecibe) >= 3){
            printf("%s", bufRecibe);       
          }
          
          fflush(stdout);
          fflush(stdin);
          memset(bufRecibe,0,MAXDATASIZE);
          recv(sockfd, bufRecibe, MAXDATASIZE, 0);
        }
        printf("\n");
  }

  printf("Client-Closing sockfd\n");
  close(sockfd);
  return 0;
}
