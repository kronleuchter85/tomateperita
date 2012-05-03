#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons-list.h"
#include "windows-commons.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"


	void doOnlineDeleteFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

	}

	void doOnlineStoreFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

	}

	void doOnlineRetrieveFile(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

	}

	void doOnlineListFiles(ClientThreadResource * res , ListenSocket clientSocket ,
		RuntimeErrorValidator * validator){

		char * pathToList = res->argument;
		List contents = callKssOperationList(pathToList , validator);
		Iterator * ite = buildIterator(contents , getGlobalHeap());

		while(hasMoreElements(ite)){
				
			if( equalsStrings(res->argument , "/")){
				char * vdaName = next(ite);

				char * msg = concat(res->heapHandler, 3, 
					"drwxrwxrwx 1 ftp ftp 4096 Jul 11 2011 ", 
					vdaName , CHARACTER_CRLF);
				
				sendBytes(clientSocket , msg , strlen(msg));

			}else{

				char * serializedContent = next(ite);				
				Iterator * ite = deserializeList(serializedContent , getGlobalHeap());
				
				char * size = hasMoreElements(ite) ? next(ite) : NULL;
				char * fileName = hasMoreElements(ite) ? next(ite) : NULL;
				
				char * msg = concat(res->heapHandler, 5 ,
					"-rwxrwxrwx 1 ftp ftp " , size , "Jul 11 2011 " , fileName , CHARACTER_CRLF);

				sendBytes(clientSocket , msg , strlen(msg));
			}
		}			
	}

