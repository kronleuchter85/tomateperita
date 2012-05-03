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
#include "kss-mps-protocol.h"
#include "kss-state.h"

	void * runServiceFtpsThread(void * arg){

		ClientConnection * conn = (ClientConnection*) arg;
		MpsRequest * req;

		do{
			req = receiveRequest(conn->clientSocket);

			if(isSysCallOpen(req)){
				executeSysCallOpen(req , conn);
			}else if( isSysCallClose(req)){
				executeSysCallClose(req, conn);
			}else if( isSysCallFlush(req)){
				executeSysCallFlush(req, conn);
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
