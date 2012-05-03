/*
 * linux-commons-socket.h
 *
 *  Created on: 30/05/2011
 *      Author: gonzalo
 */
#include <netinet/in.h>

#include "linux-commons.h"

#ifndef LINUX_COMMONS_SOCKET_H_
#define LINUX_COMMONS_SOCKET_H_

#define INVALID_SOCKET -1

	typedef int ListenSocket;

	/**
	 * Sirve como connector del lado servidor
	 */
	typedef struct {
		ListenSocket listenSocket;
		struct sockaddr_in address;
	} ServerSocket;

	typedef struct {
		ListenSocket clientSocket;
		Thread servingThread;
	} ClientConnection;


	ClientConnection * buildClientConnection(ListenSocket l);
	ServerSocket * openServerConnection(char * port);
	ListenSocket openClientConnection(char * host , char * port );
	ListenSocket acceptConnection(ServerSocket * serverSocket);

	char * receiveMessage(ListenSocket ls );
	void sendMessage(ListenSocket l , char *);

	void sendBytes(ListenSocket ls , char * bytesToSend , int bytesCount);
	void receiveBytes(ListenSocket ls , char * buffer , int bytesCount);

#endif /* LINUX_COMMONS_SOCKET_H_ */
