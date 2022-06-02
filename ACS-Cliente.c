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
