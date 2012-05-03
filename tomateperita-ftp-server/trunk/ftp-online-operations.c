#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons-list.h"
#include "windows-commons.h"
#include "windows-commons-mps.h"
#include "windows-commons-errors.h"
#include "windows-commons-memory.h"
#include "windows-commons-remote-operations.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"
#include "ftp-kss-endpoint.h"

 
	void doOnlineDeleteFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

	}

	void doOnlineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

			int handleId = callKssOperationOpenFile(res->heapHandler, res->argument , OPENING_MODE_WRITE , validator);
			FileBlock * block = buildEmptyFileBlock(res->heapHandler);
	
			block->sector1.dataLength = receiveBytes(clientSocket , block->sector1.data , sizeof(block->sector1.data)); 			
			block->sector2.dataLength = receiveBytes(clientSocket , block->sector2.data , sizeof(block->sector2.data)); 
		
			while ( (block->sector1.dataLength + block->sector2.dataLength) > 0) {

				callKssOperationWrite(res->heapHandler , handleId , block , validator);

				if(hasError(validator)){
					break;
				}

				cleanBlock(block);				
				block->sector1.dataLength = receiveBytes(clientSocket , block->sector1.data , sizeof(block->sector1.data)); 			
				block->sector2.dataLength = receiveBytes(clientSocket , block->sector2.data , sizeof(block->sector2.data)); 
			}

			callKssOperationCloseFile(res->heapHandler , handleId , validator);
			
			if (!hasError(validator)) {
				res->resultMessage = buildResponseMessageType226(res->heapHandler);
			} else {
				res->resultMessage = buildResponseMessageType426(res->heapHandler);
			}
	}




	void doOnlineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){		

			FileBlock * block = NULL;

			int handleId = callKssOperationStartReadingProcess(res->heapHandler , 
				res->argument , validator);			

			if (hasError(validator)) {
				res->resultMessage = buildResponseMessageType426(res->heapHandler);
				return;
			}
	
			block = callKssOperationRead(res->heapHandler);

			while(block != NULL){

				if(block->sector1.dataLength > 0){
					sendBytes(res->dataSocket , block->sector1.data , block->sector1.dataLength);

					if(block->sector2.dataLength > 0){
						sendBytes(res->dataSocket , block->sector2.data , block->sector2.dataLength);
					}			
				}

				block = callKssOperationRead(res->heapHandler);
			}

			callKssOperationCloseFile(res->heapHandler , handleId , validator);

			if (!hasError(validator)) {
				res->resultMessage = buildResponseMessageType226(res->heapHandler);
			} else {
				res->resultMessage = buildResponseMessageType426(res->heapHandler);
			}
	}




	void doOnlineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

		char * pathToList = res->relativeWorkingDir;
		List contents = callKssOperationList(res->heapHandler , pathToList , validator);
		Iterator * ite = buildIterator(contents , res->heapHandler);

		while(hasMoreElements(ite)){
				
			if( equalsStrings(res->relativeWorkingDir , "/")){
				char * vdaName = next(ite);

				char * msg = concat(res->heapHandler, 3, 
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", 
					vdaName , CHARACTER_CRLF);
				
				sendBytes(clientSocket , msg , (int) strlen(msg));

			}else{

				char * serializedContent = next(ite);								
				List files = deserializeList(serializedContent ,res->heapHandler);
				Iterator * ite = buildIterator(files , res->heapHandler);

				char * size = hasMoreElements(ite) ? next(ite) : NULL;
				char * fileName = hasMoreElements(ite) ? next(ite) : NULL;
				
				char * msg = concat(res->heapHandler, 5 ,
					"-rwxrwxrwx 1 ftp ftp " , size , "Jul 11 2011 " , fileName , CHARACTER_CRLF);

				sendBytes(clientSocket , msg , (int) strlen(msg));
			}
		}			

		if(hasError(validator)){
			res->resultMessage = buildResponseMessageType426(res->heapHandler);
		}else{
			res->resultMessage = buildResponseMessageType226(res->heapHandler);
		}
	}
