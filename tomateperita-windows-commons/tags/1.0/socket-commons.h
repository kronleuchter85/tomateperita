/**
 * socket-commons.h
 *
 * Define tipos y funciones comunes referentes
 * al manejo de sockets.
 *
 */


#include "headers-commons.h"
#include "memory-commons.h"



/**
 * definiciones de Estados tras las comunicacion 
 * por sockets con los metodos doSend y doReceive
 *
 */
#define SOCKET_COMM_NO_ERROR 0
#define SOCKET_COMM_ERROR -1
#define SOCKET_COMM_CLOSE 1



	/**
	 * Alias para el socket en escucha
	 */
	typedef SOCKET ListenSocket;
	typedef SOCKET ListenSocket;

	/**
	 * Funciones comunes para el manejo de sockets
	 */
	ListenSocket openServerSocket(char * hostname , char * port);
	ListenSocket openConnectionSocket(char * host , char * port);
	int closeSocket(ListenSocket s);
	int bindAndListenSocket(ListenSocket ls , struct addrinfo ** result);
	ListenSocket acceptRequest(ListenSocket serverSocket);
	char * doReceive(ListenSocket ls  , HeapHandler handler, int * error);

	int initializeWinsock();
	int finalizeWinsock();