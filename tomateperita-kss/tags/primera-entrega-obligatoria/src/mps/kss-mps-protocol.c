/*
 * kss-mps-protocol.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */
#include <string.h>
#include "kss-mps-protocol.h"
#include "kss-tdd.h"
#include "linux-commons.h"

#define FILE_PATH_SIZE 30
#define SYSCALL_FILE_BUFFER_SIZE 1024
#define SPACE_SEPARATION_IN_REQUEST ' '

	int * parseOpeningMode(char * payload);
	char * parseFilePathInOpening(char * payload);
	char * parseFileBufferContentInWrite(char * payload);
	int * parseFileDescriptor(char * payload);

	char * getVdaName(char * path);
	int getFileSize(MpsRequest* m);
	int getFileDescriptor(MpsRequest* m);
	long getIdDescriptor(char * a);



	MpsRequest * receiveRequest(ListenSocket listenSocket){

		MpsRequest * req = malloc(sizeof(MpsRequest));

		MpsMessage m;

		recv(listenSocket , &m , sizeof(MpsMessage) , 0);

		int operation = atoi(&m.payloadDescriptor);

		if( operation == SYS_OPEN){

			req->operationName = SYS_CALL_OPEN;
			req->arg1 = parseOpeningMode(m.payload);
			req->arg2 = parseFilePathInOpening(m.payload);

		}else if(operation ==SYS_CLOSE){

			req->operationName = SYS_CALL_CLOSE;
			req->arg1 = parseFileDescriptor(m.payload);

		}else if( operation == SYS_READ){

			req->operationName = SYS_CALL_READ;
			req->arg1 = parseFileDescriptor(m.payload);

		}else if (operation == SYS_WRITE){

			req->operationName = SYS_CALL_WRITE;
			req->arg1 = parseFileDescriptor(m.payload);
			req->arg2 = parseFileBufferContentInWrite(m.payload);

		}else if( operation == SYS_FLUSH){

			req->operationName = SYS_CALL_FLUSH;
			req->arg1 = parseFileDescriptor(m.payload);

		}else if(operation == SYS_FLIST){

			req->operationName = SYS_CALL_FLIST;
		}

		return req;
	}

	BOOL isSysCallRead(MpsRequest * aReq){
		return !strcmp(aReq->operationName , SYS_CALL_READ);
	}
	BOOL isSysCallWrite(MpsRequest * aReq){
		return !strcmp(aReq->operationName , SYS_CALL_WRITE);
	}
	BOOL isSysCallOpen(MpsRequest * aReq){
		return !strcmp(aReq->operationName , SYS_CALL_OPEN);
	}
	BOOL isSysCallClose(MpsRequest * aReq){
		return !strcmp(aReq->operationName , SYS_CALL_CLOSE);
	}
	BOOL isSysCallFlush(MpsRequest * aReq){
		return !strcmp(aReq->operationName , SYS_CALL_FLUSH);
	}


	void executeSysCallRead(MpsRequest * aReq, ClientConnection * conn){
	}
	void executeSysCallWrite(MpsRequest * aReq, ClientConnection * conn){
	}

	void executeSysCallOpen(MpsRequest * aReq, ClientConnection * conn){

		char * vdaName = getVdaName(aReq->arg2);
		int * openingMode = aReq->arg1;

		//TODO: se comunica con el FSS para obtener el correspondiente
		//TODO: file descriptor
		int fd = getFileDescriptor(aReq);

		//TODO: se comunica con el FSS para obtener el correspondiente
		//TODO: file size
		int fz = getFileSize(aReq);

		//TODO: si no encuentra un archivo en la tabla hace esto
		//TODO: de lo contrario lo borra y lo abre denuevo

		TddRecord * record = createTddRecord(1 , vdaName , *openingMode , 512);
		record->fileSize = fz;
		addTddRecord(record);
		long idDescriptor = getIdDescriptor(aReq->idDescriptor);

		MpsMessage m = buildMessage( idDescriptor, SUCCESS_RESULT , itoa(fd) );



	}
	void executeSysCallClose(MpsRequest * aReq, ClientConnection * conn){
	}
	void executeSysCallFlush(MpsRequest * aReq, ClientConnection * conn){
	}


	int * parseOpeningMode(char * payload){
		int * c = malloc(sizeof(int));
		*c= atoi(payload);
		return c;
	}

	char * parseFilePathInOpening(char * payload){
		char * filePath = malloc(FILE_PATH_SIZE);
		bzero(filePath , FILE_PATH_SIZE);
		strcpy(filePath , payload + 2);
		return strndup(filePath , strlen(filePath));
	}

	int * parseFileDescriptor(char * payload){
		int * n = malloc(sizeof(int));
		*n = atoi(payload);
		return n;
	}

	char * parseFileBufferContentInWrite(char * payload){
		char buffer[SYSCALL_FILE_BUFFER_SIZE];
		bzero(buffer , SYSCALL_FILE_BUFFER_SIZE);
		char * startBuff = strchr(payload , SPACE_SEPARATION_IN_REQUEST);
		strcpy(startBuff , buffer);
		return buffer;
	}


	char * getVdaName(char * path){
		return NULL;
	}

	int getFileSize(MpsRequest* m){
		return 0;
	}

	int getFileDescriptor(MpsRequest* m){
		return 0;
	}

	long getIdDescriptor(char * a){
		return 0;
	}

