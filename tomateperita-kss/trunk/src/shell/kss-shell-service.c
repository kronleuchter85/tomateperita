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
#include "linux-commons-remote-services.h"

#include "kss-shell-service.h"
#include "kss-configuration.h"
#include "kss-tdd.h"
#include "kss-state.h"
#include "kss-utils.h"

	void * runShellServiceThread(void * arg){

		if(isInfoEnabled()){
			info("Quedando a la espera de conecciones Shell");
		}

		ServerSocket * serverSocket = openServerConnection(getShellPort());
		setShellServerSocket(serverSocket);
		ListenSocket listenSocket;
		ClientConnection * clientConnection;

		Thread thread;

		RuntimeErrorValidator * validator = NULL;
		do{
			listenSocket = acceptConnection(serverSocket);
			validator = buildErrorSuccessValidator();

			doHandshake(listenSocket , SHELL_HANDSHAKE , validator);

			if(!hasError(validator)){
				clientConnection = buildClientConnection(listenSocket);
				pthread_create(&thread , NULL , runServiceShellThread , clientConnection);
				clientConnection->servingThread = thread;
			}else{
				error("No se pudo lograr el handhsake entre el kss y el proceso remoto Shell");
			}

		}while(isKssRunningStatus());

		close(getShellServerSocket());
		return EXIT_SUCCESS;
	}
