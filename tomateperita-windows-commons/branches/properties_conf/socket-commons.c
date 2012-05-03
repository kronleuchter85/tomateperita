/**
 * socket-commons.h
 *
 * Funciones comunes del manejo de sockets...
 *
 */


#include "socket-commons.h"
#include "socket-validation.h"
#include "commons-mps.h"

	/**
	 * Abre un socket y lo devuelve si tiene exito, 
	 * de lo contrario devuelve INVALID_SOCKET
	 * Esto es: 
	 * 1) crea el addrinfo, 
	 * 2) crea el socket
	 * 3) bindea el socket del sistema operativo a la aplicacion.
	 * 4) escucha en el socket
	 * 5) elimina el addrinfo que ya no sirve para nada...
	 */
	ListenSocket openServerSocket(char * hostname , char * port){
		
		ListenSocket listenSocket;
		struct addrinfo *result = NULL;
		struct addrinfo hints;
		
		int iResult;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, port, &hints, &result);
		if(validateAddressInfo(iResult) == RESULT_WITH_ERRORS ){
			return INVALID_SOCKET;
		}

		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if(validateSocketCreation(listenSocket) == RESULT_WITH_ERRORS){
			freeaddrinfo(result);
			return INVALID_SOCKET;
		}

		iResult = bindAndListenSocket(listenSocket , &result);
		if( iResult == SOCKET_ERROR){
			return INVALID_SOCKET;
		}

		return listenSocket;
	}

	ListenSocket openConnectionSocket(char * host , char * port){
		
		ListenSocket commSocket;

		int iResult;
		struct addrinfo hints;
		struct addrinfo *result = NULL;

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		iResult = getaddrinfo(host, port, &hints, &result);

		if(validateAddressInfo(iResult) == RESULT_WITH_ERRORS){
			return INVALID_SOCKET;
		}

		commSocket = socket(result->ai_family , result->ai_socktype , result->ai_protocol);

		if(validateSocketCreation(commSocket) == RESULT_WITH_ERRORS){
			freeaddrinfo(result);
			return INVALID_SOCKET;
		}

		iResult = connect(commSocket , result->ai_addr , (int)result->ai_addrlen);
		
		if(iResult == SOCKET_ERROR){
			 closesocket(commSocket);
			 freeaddrinfo(result);
			 return INVALID_SOCKET;
		}

		return commSocket;
	}


	/**
	 * bindea y escucha....
	 * si poncha devuelve SOCKET_ERROR, sino 0.
	 */
	int bindAndListenSocket(ListenSocket ls , struct addrinfo ** result){
		   
		int iResult = bind( ls, (*result)->ai_addr, (int)(*result)->ai_addrlen);

		if(validateSocketBinding(iResult) == RESULT_WITH_ERRORS){
			freeaddrinfo(*result);
			closesocket(ls);
			return SOCKET_ERROR;
		}

		freeaddrinfo(*result);

		iResult = listen(ls, SOMAXCONN);
	 
		if(validateSocketListening(iResult) == RESULT_WITH_ERRORS){
			closesocket(ls);
			return SOCKET_ERROR;
		}

		return 0;
	}

	int closeSocket(ListenSocket s){
		return closesocket(s);
	}

	/**
	 * Devuelve el socket cliente que se acaba de conectar
	 * al socket en escucha pasado como parametro...
	 */
	ListenSocket acceptRequest(ListenSocket serverSocket){
		return accept(serverSocket , NULL , NULL);
	}

	char * receiveFixedMessage(ListenSocket ls , int expectedSize , HeapHandler handler, int * error){
		char * s = NULL;
		char buffer[1024];
		int bufferPosition = 0;
		int bytesReceived;

		do{
			bytesReceived = recv(ls , buffer , expectedSize , bufferPosition); 
			bufferPosition = bufferPosition + bytesReceived;
		}while( bytesReceived < expectedSize && bytesReceived >0 );

		if(bytesReceived == expectedSize && bytesReceived > 0){

			*error = SOCKET_COMM_NO_ERROR;
			return newString(handler , buffer , bytesReceived);

		}else if(bytesReceived < 0){
			*error = SOCKET_COMM_ERROR;
		} else if(bytesReceived == 0) {
			*error = SOCKET_COMM_CLOSE;
		}
		
		return s;
	}

	char * receiveMessage(ListenSocket ls , HeapHandler handler, int * error){
		char * s = NULL;
		char buffer[1024];
		int bytesReceived = recv(ls , buffer , 1024 , 0); 

		if(bytesReceived > 0){
			*error = SOCKET_COMM_NO_ERROR;
			return newString(handler , buffer , bytesReceived);
		}else if(bytesReceived < 0){
			*error = SOCKET_COMM_ERROR;
		} else if(bytesReceived == 0) {
			*error = SOCKET_COMM_CLOSE;
		}
		
		return s;
	}

	char * receiveFileChunk(ListenSocket ls , HeapHandler handler, int * error){
		char * s = NULL;		
		char * buffer = (char *)allocateMemory(handler , FILE_TRANSFER_CHUNK);
		int bytesReceived = recv(ls , buffer , FILE_TRANSFER_CHUNK , 0);
		s = (char *)allocateMemory(handler , bytesReceived);

		strncpy_s(s,bytesReceived,buffer,bytesReceived);

		if(bytesReceived > 0){
			*error = SOCKET_COMM_NO_ERROR;
			return s;
		} else if(bytesReceived < 0){
			*error = SOCKET_COMM_ERROR;
		} else if(bytesReceived == 0) {
			*error = SOCKET_COMM_CLOSE;
		}
		
		return s;
	}

	int initializeWinsock(){
		WSADATA wsaData;
		return WSAStartup(MAKEWORD(2,2), &wsaData);
	}

	int finalizeWinsock(){
		return WSACleanup();
	}



	void sendMessage(ListenSocket ls , char * message , int * errorCode){
		*errorCode = send( ls, message, (int)strlen(message), 0 );
	}

	void sendFileChunk(ListenSocket ls , char * message , int * errorCode){
		*errorCode = send( ls, message, FILE_TRANSFER_CHUNK , 0 );
	}

	char * getNextAvailablePort(char * host , int fromPort){
		return NULL;
	}

	int sendBytes(ListenSocket ls , char * bytesToSend , int bytesCount){
		int allSended = 0;
		int sended = 0;
		while(allSended < bytesCount ){
			sended = send(ls , bytesToSend, bytesCount - allSended , 0);
			allSended += sended;
			if( sended <= 0)
				break;
		}
		return allSended;
	}

	int receiveBytes(ListenSocket ls , char * bytesToReceive , int bytesCount){
		int allReceived = 0;
		int received = 0;
		while( allReceived < bytesCount ){
			received = recv(ls , bytesToReceive , bytesCount - allReceived , 0);
			allReceived += received;
			if(received <= 0)
				break;
		}
		return allReceived;
	}

	void sendStucture(ListenSocket ls , void * structure , int length){		
		int allSsended = 0;
		int sended = 0;
		while(allSsended < length && sended >= 0 ){
			sended = send(ls , structure , length - allSsended, 0); 
			allSsended += sended;
		}
	}

	void receiveStructure(ListenSocket ls , void * structure , int size ){
		int allReceived = 0;
		int received = 0;
		
		while(allReceived < size){
			received = recv(ls , structure , size , 0);
			allReceived += received;

			if(received <= 0)
				break;
		}	
	}