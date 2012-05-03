/**
 * socket-commons.h
 *
 * Define tipos y funciones comunes referentes
 * al manejo de sockets.
 *
 */


#include "windows-commons-headers.h"
#include "windows-commons-memory.h"

  

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


	/**
	 * Funciones comunes para el manejo de sockets
	 */
	ListenSocket openServerSocket(char * hostname , char * port);
	ListenSocket openConnectionSocket(char * host , char * port);
	int closeSocket(ListenSocket s);
	int bindAndListenSocket(ListenSocket ls , struct addrinfo ** result);
	ListenSocket acceptRequest(ListenSocket serverSocket);
	void sendMessage(ListenSocket ls , char * message , int * errorCode);
	void sendFileChunk(ListenSocket , char * , int * );
	char * receiveMessage(ListenSocket ls , HeapHandler handler, int * error);
	char * receiveFixedMessage(ListenSocket ls , int expectedSize , HeapHandler handler, int * error);
	char * receiveFileChunk(ListenSocket ls , HeapHandler handler, int * error);
	int initializeWinsock();
	int finalizeWinsock();

	char * getNextAvailablePort(char * , int fromPort);

	void receiveStructure(ListenSocket ls , void * structure , int length);
	void sendStucture(ListenSocket ls , void * structure , int length);


	int receiveBytes(ListenSocket ls , void * bytesToReceive , int bytesCount);
	int sendBytes(ListenSocket ls , void * bytesToSend , int bytesCount);