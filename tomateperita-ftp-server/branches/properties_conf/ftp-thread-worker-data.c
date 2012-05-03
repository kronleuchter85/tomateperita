#include "windows-commons-socket.h"
#include "windows-commons-file.h"

#include "windows-commons.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"

	
	void storeFile(ClientThreadResource * , ListenSocket , RuntimeErrorValidator *);
	void retrieveFile(ClientThreadResource * , ListenSocket , RuntimeErrorValidator *);
	void listFiles(ClientThreadResource * , ListenSocket , RuntimeErrorValidator *);
	void deleteFile(ClientThreadResource * , ListenSocket , RuntimeErrorValidator *);


	void doDataService(ClientThreadResource * res){		
		ThreadHandler handler = _beginthreadex(NULL, 0, &serviceDataThread, res, 0 , NULL);
		res->dataThreadHandler = handler;
	}



	unsigned __stdcall serviceDataThread( void* pArguments ){
	
		ClientThreadResource * res = (ClientThreadResource*) pArguments;
		RuntimeErrorValidator * validator = buildErrorSuccessValidator(res->heapHandler);		
		ListenSocket clientDataSocket = acceptRequest(res->dataSocket);
					
		if ( res->transferTask == STOR_TRANSFER ){
			storeFile( res , clientDataSocket , validator);
		} else if ( res->transferTask == RETR_TRANSFER ){
			retrieveFile( res , clientDataSocket , validator );
		} else if ( res->transferTask == LIST_TRANSFER ){
			listFiles( res , clientDataSocket , validator);
		}			

		closeSocket(clientDataSocket);
		closeSocket(res->dataSocket);
		_endthreadex(EXIT_SUCCESS);
	}



	/*
	 * Core de la funcionalidad de almacenar archivos
	 * Este codigo es reemplazado por la llamada al ftp-kss-endpoint
	 * por MPS.
	 */
	void storeFile(ClientThreadResource * res , ListenSocket clientSocket ,RuntimeErrorValidator * validator){
		if(isOfflineStartMode()){
			doOfflineStoreFile(res , clientSocket , validator);
		}else{
			doOnlineStoreFile(res , clientSocket , validator);
		}
	}



	/*
	 * Core de la funcionalidad de obtener archivo
	 * Este codigo es reemplazado por la llamada al ftp-kss-endpoint
	 * por MPS.
	 */
	void retrieveFile(ClientThreadResource * res , ListenSocket clientSocket , RuntimeErrorValidator * validator){
		if(isOfflineStartMode()){
			doOfflineRetrieveFile(res , clientSocket , validator);
		}else{
			doOnlineRetrieveFile(res , clientSocket , validator);
		}
	}



	/*
	 * Core de la funcionalidad de listar
	 * Este codigo es reemplazado por la llamada al ftp-kss-endpoint
	 * por MPS.
	 */
	void listFiles(ClientThreadResource * res , ListenSocket clientSocket , RuntimeErrorValidator * validator){
		if(isOfflineStartMode()){
			doOfflineListFiles(res, clientSocket , validator);
		}else{
			doOnlineListFiles(res , clientSocket , validator);
		}
	}


