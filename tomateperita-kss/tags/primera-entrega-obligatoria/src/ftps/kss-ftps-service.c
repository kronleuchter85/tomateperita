/*
 * kss-ftps-service.c
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

#include "kss-state.h"
#include "kss-configuration.h"
#include "kss-ftps-service.h"
#include "linux-commons-socket.h"
#include "kss-ftps-worker-thread.h"
#include "kss-internal-state.h"
#include "kss-utils.h"


	void * runFtpServiceThread(void * arg){

		ServerSocket * serverSocket = openServerConnection(getFtpsPort());
		ListenSocket listenSocket;
		ClientConnection * clientConnection;

		Thread thread;

		setFtpsServerConnection(serverSocket);

		do{
			if(isKssOperationalStatus()){

				if(isInfoEnabled()){
					info("Quedando a la espera de conecciones FTP");
				}

				listenSocket = acceptConnection(serverSocket);
				clientConnection = malloc(sizeof(ClientConnection));
				clientConnection->clientSocket = listenSocket;

				pthread_create(&thread , NULL , runServiceFtpsThread , clientConnection);
				clientConnection->servingThread = thread;

				//TODO: agregar a la lista de recursos este ClientConnection
				//TODO: representando a uno de los clinetes FTP conectados
			}else{

				if(isInfoEnabled()){
					info("Aguardando por estado operacional");
				}
				sleep(5);
			}

		}while(isKssRunningStatus());

		closeFtpsServerConnection();

		return EXIT_SUCCESS;
	}


