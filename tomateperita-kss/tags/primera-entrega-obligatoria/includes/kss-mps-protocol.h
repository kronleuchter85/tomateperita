/*
 * kss-mps-protocol.h
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include "linux-commons-socket.h"
#include "linux-commons-mps.h"

#ifndef KSS_MPS_PROTOCOL_H_
#define KSS_MPS_PROTOCOL_H_


#define SYS_CALL_OPEN 	"sys_open"
#define SYS_CALL_CLOSE 	"sys_close"
#define SYS_CALL_READ 	"sys_read"
#define SYS_CALL_WRITE	"sys_write"
#define SYS_CALL_FLUSH	"sys_flush"
#define SYS_CALL_FLIST	"sys_flist"

	typedef struct {

		char idDescriptor[16];
		char * operationName;
		void * arg1;
		void * arg2;

	}MpsRequest;

	typedef struct {

		char statusCode;
		char * resultValue;

	}MpsResponse;


	MpsRequest * receiveRequest(ListenSocket listenSocket);
	MpsResponse * buildMpsResponse(char statusCode);
	void sendResponse(ListenSocket , MpsResponse);

	BOOL isSysCallRead(MpsRequest *);
	BOOL isSysCallWrite(MpsRequest *);
	BOOL isSysCallOpen(MpsRequest *);
	BOOL isSysCallClose(MpsRequest *);
	BOOL isSysCallFlush(MpsRequest *);

	void executeSysCallRead(MpsRequest * , ClientConnection *);
	void executeSysCallWrite(MpsRequest *, ClientConnection *);
	void executeSysCallOpen(MpsRequest *, ClientConnection *);
	void executeSysCallClose(MpsRequest *, ClientConnection *);
	void executeSysCallFlush(MpsRequest *, ClientConnection *);


#endif /* KSS_MPS_PROTOCOL_H_ */
