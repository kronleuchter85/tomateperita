/*
 * kss-shell-service.c
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "linux-commons-socket.h"
#include "linux-commons-logging.h"

#include "kss-shell-service.h"
#include "kss-configuration.h"
#include "kss-tdd.h"
#include "kss-state.h"

	void * runShellServiceThread(void * arg){

		if(isInfoEnabled()){
			info("Quedando a la espera de conecciones Shell");
		}

		ServerSocket * serverSocket = openServerConnection(getShellPort());
		ListenSocket listenSocket;
		ClientConnection * clientConnection;

		Thread thread;

		do{
			listenSocket = acceptConnection(serverSocket);
			clientConnection = buildClientConnection(listenSocket);
			pthread_create(&thread , NULL , runServiceShellThread , clientConnection);
			clientConnection->servingThread = thread;

			//TODO: agregar a la lista de recursos este ClientConnection
			//TODO: representando a uno de los clinetes shell conectados

		}while(isKssRunningStatus());

		return EXIT_SUCCESS;
	}
