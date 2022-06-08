/*Proyecto Final de la Materia de Arquitectura Cliente-Servidor
Objetivo: Crear un Cliente-Servidor que ejecute comandos
remotamente como ocurre con un cliente-Servidor SSH comercial 
Cortes Lopez Maricela
Hernandez Calderon Fernando
Archivo Servidor*/

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
#include <signal.h>

//Puerto al que nos queremos conectar
#define ELPUERTO 3490
//Establecemos un maximo de bytes que puede recibir a la vez
#define MAXDATASIZE 300
//El tamaño de cola de escucha
#define BACKLOG 5

//Variable global para cerrar el servidor al terminar
int flag = 0;

int conexion(int new_fd){
	int numbytes;
	// buf sera la entrada del usuario
	char buf[MAXDATASIZE];
	// salida sera el resultado del comando
	char salida[MAXDATASIZE];
	// comando es el apuntador al resultado del shell
	FILE *comando;

	while( strcmp (buf,"exit\n") != 0){
			memset(buf,0,MAXDATASIZE);
			//Verificamos que el mensaje recibido no tiene algun error
			if((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1){
				perror("recv()");
				exit(1);
			}
		
			// Ponemos fin de cadena al mensaje
			buf[numbytes] = '\0';
			printf("Servidor- Recibe: %s \n", buf);

			//Mandamos a llamar al shell para ejecutar el comando
			fflush(stdout);
			if((comando = popen(buf,"r")) == NULL ){
				perror("popen error");
			}

			// Enviamos resultado al cliente
			while (fgets(salida, MAXDATASIZE, comando) != NULL){
    			printf("Servidor- Envia: %s", salida);
    			send(new_fd, salida, MAXDATASIZE, 0);
    		}
    		// Limpiamos despues de haber enviado resultado
    		fflush(stdout);
    		fflush(stdin);
    		// Enviamos mensaje que termino el resultado
    		send(new_fd, "termine\n", MAXDATASIZE, 0);
    		// Cerramos el FILE comando
			pclose(comando);


		}
		send(new_fd, "Conexion cerrada\n", MAXDATASIZE, 0);
		//Cerramos el socket de coneccion con el cliente
		close(new_fd);
		printf("Server-new socket, new_fd closed successfully...\n");
}

void manejador_senales(int sig){
  if( sig == SIGINT ){
  		printf("\nCerrando servidor, adios\n");
  		close(flag);
  		exit(1);
  }
}

int main(int argc, char *argv[ ]){
	//Variables para la escucha del socket, nuevas conexiones y num de bytes
	int sockfd, new_fd, numbytes;
	//Estructura donde se guarda la info de nuestra direccion
	struct sockaddr_in my_addr;
	// Informacion de la direccion de los conectores
	struct sockaddr_in their_addr;
	int sin_size;
	

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

	if (signal(SIGINT,manejador_senales) == SIG_ERR){
		printf("no puedo cachar SIGINT\n");
    	perror("signal");
	}

	flag = sockfd;
	// Inicia el servidor
	while(1){
		// Intentamos crear la conexion con el cliente
		if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("Server-accept() error");
			exit(1);
		}else{
			printf("Servidor-Conexion exitosa...\n");
			printf("Servidor: Conexion entrante de:  %s\n", inet_ntoa(their_addr.sin_addr));
			//Funcion de conexion 
			conexion(new_fd);
		}
	}
	printf("Cerrando servidor, adios\n");
	//Cerramos el descriptor de archivo de la conexion
	close(sockfd);
	return 0;
}