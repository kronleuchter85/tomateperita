/*
 * kss-ftps-worker-thread.c
 *
 *  Created on: 31/05/2011
 *      Author: gonzalo
 */
#include <unistd.h>
#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-socket.h"
#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-errors.h"

#include "kss-ftps-entrypoint.h"
#include "kss-state.h"



	void executeFtpCommand(ListenSocket l , MpsRequest * req);


	void * runServiceFtpsThread(void * arg){
		RuntimeErrorValidator * validator = NULL;
		ClientConnection * conn = (ClientConnection*) arg;
		MpsRequest * req = NULL;

		do{
			validator = buildErrorSuccessValidator();
			req = receiveMpsRequest(conn->clientSocket , validator);

			executeFtpCommand(conn->clientSocket , req);

		}while(isKssRunningStatus() && isKssOperationalStatus());

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}





	void executeFtpCommand(ListenSocket l , MpsRequest * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		if(equalsStrings(req->operationName , KSS_SYS_CALL_OPEN)){
			executeOperationOpen(l ,req , validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_CLOSE)){
			executeOperationClose(l ,req, validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_READ)){
			executeOperationRead(l ,req, validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_WRITE)){
			executeOperationWrite(l ,req, validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_FLIST)){
			executeOperationList(l , req , validator);
		}
	}
