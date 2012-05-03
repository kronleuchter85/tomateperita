/*
 * kss-fss-service.c
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
#include "linux-commons-logging.h"
#include "linux-commons-remote-services.h"

#include "kss-fss-service.h"
#include "kss-configuration.h"
#include "kss-state.h"
#include "kss-utils.h"




	void * runFssServiceThread(void * arg) {

		if(isInfoEnabled()){
			info("Quedando a la espera de conecciones FSS");
		}

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		ServerSocket * serverSocket = openServerConnection(getFssListenPort());
		setFssServerSocket(serverSocket);
		ListenSocket clientSocket = acceptConnection(serverSocket);

		doHandshake(clientSocket , FSS_HANDSHAKE, validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return NULL;
		}

		setFileSystemConnection(clientSocket );
		close(serverSocket);

		return EXIT_SUCCESS;
	}
