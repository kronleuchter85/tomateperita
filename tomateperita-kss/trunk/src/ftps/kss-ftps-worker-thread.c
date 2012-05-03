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
#include "kss-sys-calls.h"

#include "kss-state.h"



	void executeFtpCommand(ListenSocket l , MpsMessage * req);


	void * runServiceFtpsThread(void * arg){
		RuntimeErrorValidator * validator = NULL;
		ClientConnection * conn = (ClientConnection*) arg;
		MpsMessage * req = NULL;

		do{
			validator = buildErrorSuccessValidator();
			req = receiveMpsMessage(conn->clientSocket , validator);

			/*
			 * Si el mensaje viene en null es
			 * porque el socket se cerró. Por lo tanto
			 * liberamos el socket y la memoria y dejamos
			 * de atender a este cliente.
			 */
			if(req == NULL )
				break;

			executeFtpCommand(conn->clientSocket , req);

		}while( isKssRunningStatus() && isKssOperationalStatus());

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}





	void executeFtpCommand(ListenSocket l , MpsMessage * req){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		if(equalsStrings(req->operationName , KSS_SYS_CALL_OPEN)){
			executeOperationOpen(l ,req , validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_CLOSE)){
			executeOperationClose(l ,req, validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_READ)){
			executeOperationRead(l ,req, validator);
		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_WRITE)){

			req = receiveMpsBlockMessage(l , validator);

			if(! hasError(validator)){
				executeOperationWrite(l ,req, validator);
			}

		}else if( equalsStrings(req->operationName , KSS_SYS_CALL_FLIST)){
			executeOperationList(l , req , validator);
		}
	}
