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

//Puerto al que nos queremos conectar
#define ELPUERTO 3490
//Establecemos un maximo de bytes que puede recibir a la vez
#define MAXDATASIZE 300
#define BACKLOG 10

int main(int argc, char *argv[ ]){
	//Variables para la escucha del socket, nuevas conexiones y num de bytes
	int sockfd, new_fd, numbytes;
	//Estructura donde se guarda la info de nuestra direccion
	struct sockaddr_in my_addr;
	// Informacion de la direccion de los conectores
	struct sockaddr_in their_addr;
	int sin_size;
	//Definimos la capacidad del buffer
	char buf[MAXDATASIZE];
	
	//Comprobamos si hay un error con el descriptor de fichero
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Server-socket() error lol!");
		exit(1);
	} else{
		printf("Servidor-El socket funciona\n");
	}

	//Configuramos la estructura de la informacion del server
	my_addr.sin_family = AF_INET; //conjunto de direcciones a las que se puede conectar
	my_addr.sin_port = htons(ELPUERTO); //Puerto por el que se conectara
	my_addr.sin_addr.s_addr = INADDR_ANY; //Recibe de cualquer de nuestras interfaces
	//Ponemos en 0 o "null" el resto de la estructura
	memset(&(my_addr.sin_zero), '\0', 8);
	printf("El servidor esta en %s Con el puerto %d...\n", inet_ntoa(my_addr.sin_addr), ELPUERTO);
	
	// Le intentamos asignar al socket sockfd una direccion de socket local
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
		perror("Server-bind() error");
		exit(1);
	} else{
		printf("Servidor-Se enlazo correctamente\n");
	}

	//Comprobamos si el servidor puede escuchar el socket
	if(listen(sockfd, BACKLOG) == -1){
		perror("Server-listen() error");
		exit(1);
	}else {
		printf("Servidor-Escucha...\n");
	}
	
	// obtenemos el tamaño de la estructura 
	sin_size = sizeof(struct sockaddr_in);
	
	while(1){
		// Comprobamos si la conexion se realizo correctamente
		if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("Server-accept() error");
			exit(1);
		}else{
			printf("Servidor-Conexion exitosa...\n");
			//printf("Server-new socket, new_fd is OK...\n");
			printf("Servidor: Conexion entrante de:  %s\n", inet_ntoa(their_addr.sin_addr));
		}

		//Verificamos que el mensaje recibido no tiene algun error
		if((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1){
			perror("recv()");
			exit(1);
		} else {
			printf("Servidor-Mensaje recibido ...\n");
		}
		
		//Ponemos fin de cadena al mensaje
		buf[numbytes] = '\0';
		printf("Servidor-Received: %s", buf);
		
		//Cerramos el 
		close(new_fd);
		printf("Server-new socket, new_fd closed successfully...\n");
	}
	
	//Cerramos el descriptor de archivo de la conexion
	close(sockfd);
	return 0;
}
