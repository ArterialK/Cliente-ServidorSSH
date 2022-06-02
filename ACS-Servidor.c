/*Proyecto Final de la Materia de Arquitectura Cliente-Servidor
Objetivo: Crear un Cliente-Servidor que ejecute comandos
remotamente como ocurre con un cliente-Servidor SSH comercial */

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

#define BACKLOG 10

int main(int argc, char *argv[ ]){
	//Variables para la escucha del socket, nuevas conexiones y num de bytes
	int sockfd, new_fd, numbytes;
	//Estructura donde se guarda la info de nuestra direccion
	struct sockaddr_in my_addr;
}