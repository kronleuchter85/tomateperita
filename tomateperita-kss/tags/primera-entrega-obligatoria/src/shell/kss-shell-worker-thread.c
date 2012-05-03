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
#include "kss-tdd.h"
#include "kss-shell-service.h"


	void * runServiceShellThread(void * arg){

		ClientConnection * conn = (ClientConnection*)arg;
		char * cmd = NULL;

		do{
			cmd = receiveMessage(conn->clientSocket);

			if( !isCommandQuit(cmd))
				executeCommand(cmd);

		}while(!isCommandQuit(cmd));

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}
