/*
 * kss-shell-worker-thread.c
 *
 *  Created on: 31/05/2011
 *      Author: gonzalo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linux-commons-socket.h"
#include "linux-commons-mps.h"

#include "kss-tdd.h"
#include "kss-shell-service.h"




	Bool isCommandQuit(char * cmd){
		return startsWith("quit" , cmd);
	}

	void * runServiceShellThread(void * arg){
		RuntimeErrorValidator * validator ;
		ClientConnection * conn = (ClientConnection*)arg;
		char * cmd = NULL;

		MpsRequest * request = NULL;

		do{
			validator = buildErrorSuccessValidator();
			request = receiveMpsRequest(conn->clientSocket , validator);
			cmd = request->operationName;

			if( !isCommandQuit(cmd))
				executeCommand(conn->clientSocket , request);

		}while(!isCommandQuit(cmd));

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}
