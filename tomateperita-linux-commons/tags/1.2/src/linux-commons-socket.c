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
#include "linux-commons-mps.h"
#include "linux-commons-logging.h"

	ServerSocket * openServerConnection(char * port) {

		ServerSocket * serverSocket = (ServerSocket *) malloc(sizeof(ServerSocket));

		serverSocket->listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (serverSocket->listenSocket == SOCKET_ERROR) {

			logError("linux-commons" , "No se pudo crear el socket con la llamada 'socket'");
			return NULL;
		}

		bzero(&serverSocket->address, sizeof(struct sockaddr_in));
		serverSocket->address.sin_addr.s_addr = INADDR_ANY;
		serverSocket->address.sin_port = (in_port_t) htons(atoi(port));
		serverSocket->address.sin_family = AF_INET;

		if (bind(serverSocket->listenSocket,
				(struct sockaddr*) &serverSocket->address,
				sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
			logError("linux-commons" , "no se pudo bindear el socket");
			return NULL;
		}

		if (listen(serverSocket->listenSocket, 5) == SOCKET_ERROR) {
			logError("linux-commons" , "no se pudo poner el socket en escucha");
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

		if(host == NULL || port == NULL){
			logError("linux-commons" , "invocando 'openClientConnection' con parametros {host} o {port} en NULL");
			return INVALID_SOCKET;
		}

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

	void sendMpsStream(ListenSocket l , MpsStream * s){
		int allSended = 0;
		int currentllySended = 0;
		while(allSended < s->length && currentllySended >=0 ){
			currentllySended = send(l , s->data , s->length - allSended, 0);
			allSended += currentllySended;
		}
	}


	ClientConnection * buildClientConnection(ListenSocket l){
		ClientConnection * clientConnection = malloc(sizeof(ClientConnection));
		clientConnection->clientSocket = l;
		return clientConnection;
	}

	void sendBytes(ListenSocket ls , char * bytesToSend , int bytesCount){
		int allSsended = 0;
		int sended = 0;
		while(allSsended < bytesCount && sended >= 0 ){
			sended = send(ls , bytesToSend, bytesCount - allSsended , 0);
			allSsended += sended;
		}

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3 , "Se han enviado " , itoa(allSsended) , " bytes"));
	}

	void receiveBytes(ListenSocket ls , char * bytesToReceive , int bytesCount){
/*		char buffer[2048];
		bzero(buffer, 2048);
		int received = 0;
		while( received != bytesCount && received >= 0){
			received = recv(ls , buffer , bytesCount , 0);
			if(received != bytesCount)
				bzero(buffer , 2048);
		}

		strcpy(bytesToReceive , buffer);
*/

		int allReceived = 0;
		int received = 0;
		while( allReceived < bytesCount && received >= 0){
			received = recv(ls , bytesToReceive , bytesCount - allReceived  , 0);
			allReceived += received;
		}

		if(isDebugEnabled())
			logDebug("linux-commons" , concatAll(3, "Se han recivido " , itoa(allReceived) , " bytes") );
	}


