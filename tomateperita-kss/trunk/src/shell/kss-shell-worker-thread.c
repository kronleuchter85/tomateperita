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
#include "linux-commons-remote-services.h"

#include "kss-tdd.h"
#include "kss-shell-service.h"
#include "kss-sys-calls.h"


	void executeShellCommand(ListenSocket l , MpsMessage * req , RuntimeErrorValidator * v);


	void * runServiceShellThread(void * arg){
		RuntimeErrorValidator * validator ;
		ClientConnection * conn = (ClientConnection*)arg;
		char * cmd = NULL;

		MpsMessage * request = NULL;

		do{
			validator = buildErrorSuccessValidator();
			request = receiveMpsMessage(conn->clientSocket , validator);

			if(request == NULL )
				break;

			cmd = request->operationName;

			executeShellCommand(conn->clientSocket , request , validator);

		}while(!isCommandQuit(cmd));

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}



	Bool isCommandQuit(char * cmd){
		return startsWith(SHELL_QUIT , cmd);
	}


	void executeShellCommand(ListenSocket l , MpsMessage * req , RuntimeErrorValidator * v){

		if(equalsStrings(SHELL_MOUNT , req->operationName)){

			executeCommandMount(l , req);
		}else if(equalsStrings(SHELL_UMOUNT ,req->operationName)){

			executeCommandUmount(l , req);
		}else if(equalsStrings(SHELL_FORMAT ,req->operationName)){

			executeCommandFormat(l , req);
		}else if(equalsStrings(SHELL_LS,req->operationName)){

			executeCommandList(l , req);
		}else if(equalsStrings(SHELL_TDD_DUMP, req->operationName)){

			executeCommandTddDump(l , req);
		}else if(equalsStrings(SHELL_DISCONNECT , req->operationName)){

			executeCommandDisconnect(l , req);
		}else if(equalsStrings(SHELL_SHOW_DEVICES , req->operationName)){

			executeCommandShowDevices(l , req);
		}else if(equalsStrings(KSS_SYS_CALL_OPEN , req->operationName)){

			executeOperationOpen(l , req , v);
		}else if(equalsStrings(KSS_SYS_CALL_READ , req->operationName)){

			executeOperationRead(l , req , v);
		}else if(equalsStrings(KSS_SYS_CALL_CLOSE , req->operationName)){

			executeOperationClose(l , req , v);
		}else if(equalsStrings(SHELL_QUIT , req->operationName)){

			executeCommandQuit(l , req);
		}
	}
