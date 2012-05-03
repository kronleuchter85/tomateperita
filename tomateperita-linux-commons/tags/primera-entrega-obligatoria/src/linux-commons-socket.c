/*
 * linux-commons-socket.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "linux-commons-socket.h"

	ServerSocket * openServerConnection(char * port) {

		ServerSocket * serverSocket = (ServerSocket *) malloc(sizeof(ServerSocket));

		serverSocket->listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (serverSocket->listenSocket == SOCKET_ERROR) {
			//TODO: sacar este printf y loguear como corresponde
			printf("\nCould not socket\n");
			return NULL;
		}

		bzero(&serverSocket->address, sizeof(struct sockaddr_in));
		serverSocket->address.sin_addr.s_addr = INADDR_ANY;
		serverSocket->address.sin_port = (in_port_t) htons(atoi(port));
		serverSocket->address.sin_family = AF_INET;

		if (bind(serverSocket->listenSocket,
				(struct sockaddr*) &serverSocket->address,
				sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
			//TODO: sacar este printf y loguear como corresponde
			printf("\nCould not bind\n");
			return NULL;
		}

		if (listen(serverSocket->listenSocket, 5) == SOCKET_ERROR) {
			//TODO: sacar este printf y loguear como corresponde
			printf("\nCould not listen\n");
			return NULL;
		}

		return serverSocket;
	}

	ListenSocket acceptConnection(ServerSocket * serverSocket) {
		int nAddressSize = sizeof(struct sockaddr_in);
		return accept(serverSocket->listenSocket,
				(struct sockaddr*) &serverSocket->address,
				(socklen_t *) &nAddressSize);
	}

	char * receiveMessage(ListenSocket ls ){
		char buffer[1024];
		bzero(buffer,1024);
		int received = read(ls , buffer , 1024 );
		char * s = malloc(received);
		strcpy(s , buffer);
		return s;
	}


	ListenSocket openClientConnection(char * host , char * port ){

		struct hostent * serverHost;
		struct sockaddr_in clientSocket;
		bzero((char *) &clientSocket , sizeof(clientSocket));
		serverHost = gethostbyname(host);
		clientSocket.sin_port = htons(atoi(port));
		clientSocket.sin_family = AF_INET;
		clientSocket.sin_addr.s_addr = ((struct in_addr*)serverHost->h_addr_list[0])->s_addr;
		ListenSocket clientDescriptor = socket(AF_INET , SOCK_STREAM , IPPROTO_IP);
		int status = connect(clientDescriptor , (struct  sockaddr *)&clientSocket , sizeof(clientSocket));

		return (status == 0)? clientDescriptor : INVALID_SOCKET;
	}

	void sendMessage(ListenSocket l , char * message){
		send(l , message , strlen(message) , 0);
	}

	ClientConnection * buildClientConnection(ListenSocket l){
		ClientConnection * clientConnection = malloc(sizeof(ClientConnection));
		clientConnection->clientSocket = l;
		return clientConnection;
	}

