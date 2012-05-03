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

#include "kss-state.h"


	Bool isSysCallRead(MpsRequest * aReq);
	Bool isSysCallWrite(MpsRequest * aReq);
	Bool isSysCallOpen(MpsRequest * aReq);
	Bool isSysCallClose(MpsRequest * aReq);


	void executeSysCallRead(MpsRequest * aReq, ClientConnection * conn);
	void executeSysCallWrite(MpsRequest * aReq, ClientConnection * conn);
	void executeSysCallOpen(MpsRequest * aReq, ClientConnection * conn);
	void executeSysCallClose(MpsRequest * aReq, ClientConnection * conn);
	void executeSysCallFlush(MpsRequest * aReq, ClientConnection * conn);


	void * runServiceFtpsThread(void * arg){
		ClientConnection * conn = (ClientConnection*) arg;
		RuntimeErrorValidator * validator ;
		MpsRequest * req = NULL;
		do{
			validator = buildErrorSuccessValidator();
			req = receiveMpsRequest(conn->clientSocket , validator);

			if(isSysCallOpen(req)){
				executeSysCallOpen(req , conn);
			}else if( isSysCallClose(req)){
				executeSysCallClose(req, conn);
			}else if( isSysCallRead(req)){
				executeSysCallRead(req, conn);
			}else if( isSysCallWrite(req)){
				executeSysCallWrite(req, conn);
			}
		}while(isKssRunningStatus() && isKssOperationalStatus());

		close(conn->clientSocket);
		free(conn);

		return EXIT_SUCCESS;
	}


	Bool isSysCallRead(MpsRequest * aReq){
		return equalsStrings(aReq->operationName , KSS_SYS_CALL_READ);
	}
	Bool isSysCallWrite(MpsRequest * aReq){
		return equalsStrings(aReq->operationName , KSS_SYS_CALL_WRITE);
	}
	Bool isSysCallOpen(MpsRequest * aReq){
		return equalsStrings(aReq->operationName , KSS_SYS_CALL_OPEN);
	}
	Bool isSysCallClose(MpsRequest * aReq){
		return equalsStrings(aReq->operationName , KSS_SYS_CALL_CLOSE);
	}


	void executeSysCallRead(MpsRequest * aReq, ClientConnection * conn){
	}
	void executeSysCallWrite(MpsRequest * aReq, ClientConnection * conn){
	}
	void executeSysCallOpen(MpsRequest * aReq, ClientConnection * conn){
	}
	void executeSysCallClose(MpsRequest * aReq, ClientConnection * conn){
	}





