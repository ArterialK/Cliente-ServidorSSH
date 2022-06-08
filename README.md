# Cliente-ServidorSSH
Codigos de archivos cliente-servidor con funciones similares a una conexion SSH

Para la materia Arquitectura Cliente Servidor

Semestre 2022-2

Facultad de Ingenieria

UNAM


Desarrollado en lenguaje C para ambiente Linux. Probado en Ubuntu. El servidor escucha por el puerto 3490, se mantiene a la escucha de cualquier cliente por este puerto.

Una vez conectado el cliente puede ejecutar la mayoria de los comandos donde el servidor le respondera con el resultado del comando en la maquina donde se esta ejecutando el servidor simulando asi una conexion SSH donde la informacion que se obtiene es de la maquina del servidor en lugar de la del cliente.

Una vez terminada la conexion con el cliente por medio de la palabra "exit" el servidor se mantendra a la escucha de un nuevo cliente.
