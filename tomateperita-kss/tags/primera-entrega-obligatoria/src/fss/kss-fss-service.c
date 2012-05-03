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
#include "kss-fss-service.h"
#include "kss-configuration.h"
#include "kss-internal-state.h"
#include "kss-state.h"
#include "kss-utils.h"

	void * runFssServiceThread(void * arg) {

		if(isInfoEnabled()){
			info("Quedando a la espera de conecciones FSS");
		}

		ServerSocket * serverSocket = openServerConnection(getFssListenPort());
		ListenSocket clientSocket = acceptConnection(serverSocket);

		setFssComponentConnection(clientSocket );
		closeAndFreeServerConnection(serverSocket);

		return EXIT_SUCCESS;
	}
