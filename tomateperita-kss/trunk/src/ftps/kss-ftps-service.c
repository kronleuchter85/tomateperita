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

#include "linux-commons-socket.h"
#include "linux-commons-logging.h"
#include "linux-commons-remote-services.h"

#include "kss-state.h"
#include "kss-configuration.h"
#include "kss-ftps-service.h"
#include "kss-ftps-worker-thread.h"
#include "kss-utils.h"


	void * runFtpServiceThread(void * arg){

		ServerSocket * serverSocket = openServerConnection(getFtpsPort());
		setFtpServerSocket(serverSocket);
		ListenSocket listenSocket;
		ClientConnection * clientConnection;
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		Thread thread;

		do{
			if(isKssOperationalStatus()){


				if(isInfoEnabled()){
					info("Quedando a la espera de conecciones FTP");
				}

				listenSocket = acceptConnection(serverSocket);
				doHandshake(listenSocket , FTP_HANDSHAKE , validator);

				if(hasError(validator)){
					error("No se pudo lograr el handhsake entre el kss y el proceso remoto FTPS.");
				} else {
					clientConnection = malloc(sizeof(ClientConnection));
					clientConnection->clientSocket = listenSocket;

					pthread_create(&thread , NULL , runServiceFtpsThread , clientConnection);
					clientConnection->servingThread = thread;
				}

			}else{

				if(isInfoEnabled()){
					info("Aguardando por estado operacional");
				}
				sleep(5);
			}

		}while(isKssRunningStatus());


		close(getFtpServerSocket());
		return EXIT_SUCCESS;
	}


