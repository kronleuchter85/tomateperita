#include "commons.h"
#include "headers-commons.h"
#include "ftp-constants.h"
#include "ftp-service-validation.h"
#include "ftp-thread-worker.h"
#include "socket-commons.h"
#include "socket-validation.h"
#include "memory-commons.h"
#include "ftp-client-thread-resources.h"
#include "logging.h"
#include "ftp-protocol-constants.h"
#include "ftp-protocol-service.h"
#include "ftp-configuration.h"
#include "ftp-commons.h"
#include "ftp-data-thread-worker.h"


	void doDataService(ClientThreadResource * );
	void validateAndCloseDataConnection(ClientThreadResource * );

	void executeCommandPwd(ClientThreadResource *  , char *  , int * );
	void executeCommandUser(ClientThreadResource *  , int * );
	void executeCommandSyst(ClientThreadResource *  , int * );
	void executeCommandNotImplemented(ClientThreadResource *  , int * );
	void executeCommandType(ClientThreadResource *  , char *  , int * );
	void executeCommandPasv(ClientThreadResource *  , int * );
	void executeCommandPort(ClientThreadResource * , char * , int *  );
	void executeCommandList(ClientThreadResource *, int *);
	void executeCommandStore(ClientThreadResource *, char * , int *);	//FALTA IMPLEMENTARLA Pr1
	void executeCommandRetrieve(ClientThreadResource *, char * , int *); //FALTA IMPLEMENTARLA Pr1
	void executeCommandCwd(ClientThreadResource *, int *); //FALTA IMPLEMENTARLA Pr2


	void execute(char * aRequest , ClientThreadResource * aResource , 
		char * workPath , char * clientDtpPort , int * iResult){
			
			if( startsWith(aRequest , COMMAND_USER)){
				if(isAnonymousAccess()){
					executeCommandUser( aResource , iResult);
				}
			}else if(startsWith(aRequest, COMMAND_TYPE)){
				executeCommandType(aResource , "I" , iResult);
			}else if( startsWith(aRequest , COMMAND_PWD)){
				executeCommandPwd( aResource , workPath , iResult);
			}else if( startsWith(aRequest , COMMAND_LIST)){
				executeCommandList(aResource , iResult);

			}else if( startsWith(aRequest , COMMAND_RETRIEVE)){
				executeCommandRetrieve(aResource , aRequest , iResult);		//FALTA IMPLEMENTARLA Pr1

			}else if( startsWith(aRequest , COMMAND_STORE)){
				executeCommandStore(aResource, aRequest , iResult);			//FALTA IMPLEMENTARLA Pr1
			
			}else if(startsWith(aRequest, COMMAND_SYST)){
				executeCommandSyst(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_PASSIVE)){
				executeCommandPasv(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_PORT)){
				executeCommandPort(aResource , aRequest , iResult);
			}else{
				executeCommandNotImplemented(aResource , iResult);
			}
	}

	void executeCommandUser(ClientThreadResource * aResource , int * iResult){
		char * message = concat(aResource->heapHandler , 3 , RESPONSE_CODE_230 ,  
			" usuario conectado " , CHARACTER_CRLF);

		sendMessage(aResource->controlSocket , message , &iResult);
	}

	void executeCommandPwd(ClientThreadResource * aRes , char * workingDir , int * iResult){		

		char * message = concat(aRes->heapHandler , 5 , RESPONSE_CODE_257 , 
			CHARACTER_SP , workingDir , CHARACTER_SP , CHARACTER_CRLF);
		printf("%s",message);
		sendMessage(aRes->controlSocket , message , iResult);
	}

	void executeCommandSyst(ClientThreadResource * res , int * iresult){
		char * m = concat(res->heapHandler , 2 , "215 Windows XP " , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &iresult);
	}

	void executeCommandNotImplemented(ClientThreadResource * res , int * result){
		char * m = concat(res->heapHandler , 2 , 
			"502 Orden no implementada, no necesaria en este sistema " , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &result);
	}

	void executeCommandType(ClientThreadResource * res , char * type , int * result){
		char * m = concat(res->heapHandler , 2 , 
			"200 orden ok, type... " , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &result);
	}
	
	void executeCommandPasv(ClientThreadResource * res , int * result){
		
		int cantRetries = 0;
		char * port = calculatePort(getServerDataPort() , res->heapHandler);
		
		while( (res->dataSocket = openServerSocket(getServerAddress() , port)) == INVALID_SOCKET
			&& cantRetries < getMaxRetriesFindingDataPort()){
				port = calculatePort(port , res->heapHandler);
				cantRetries++;
		}

		validateAndCloseDataConnection(res);

		res->mode = FTP_DATA_MODE_PASIVE;

		doDataService(res);

		sendMessage(res->controlSocket , concat(res->heapHandler , 3 ,
			"227 " , encodeHostAndPort(getServerAddress() , port , res->heapHandler)
			, CHARACTER_CRLF) , &result);
	}

	void executeCommandPort(ClientThreadResource * res , char * aRequest , int * result ){		
		
		char * hp = parseHostAndPort(aRequest , res->heapHandler);
		char * host = decodeHost(hp, res->heapHandler);
		char * port = decodePort(hp , res->heapHandler);

		validateAndCloseDataConnection(res);

		//res->dataSocket = openConnectionSocket(host , port);		
		res->remoteHost = host;
		res->remotePort = port;
		res->mode = FTP_DATA_MODE_ACTIVE;
		
		doDataService(res);

		sendMessage(res->controlSocket , concat(res->heapHandler , 2 , 
			"200 modo activo iniciado " , CHARACTER_CRLF) , &result);	
	}

	void executeCommandList(ClientThreadResource * res , int * result){
		res->commandToExecute = COMMAND_LIST;
		sendMessage(res->controlSocket , concat(res->heapHandler , 2 , 
			"150 iniciando transferencia de datos " , CHARACTER_CRLF) , &result);

		unblockMutex(res->dataMutex);		
		blockMutex(res->controlMutex);

		res->argument = getDataDirectory();
		res->transferTask = LIST_TRANSFER;

		ResumeThread( res->dataThreadHandler );
		WaitForSingleObject( res->dataThreadHandler , INFINITE );

		//sendMessage(res->controlSocket , concat(res->heapHandler , 3 , 
			//res->resultMessage  , " " , CHARACTER_CRLF ), &result);
	}

	void executeCommandRetrieve(ClientThreadResource * res , char * request , int * result){
		request = strstr( request , " ");
		request++;
		_tprintf("RETR %s",request);
	    
		ResumeThread( res->dataThreadHandler );
		printf("Thread liberado...");
		WaitForSingleObject( res->dataThreadHandler, INFINITE );

	}

	void executeCommandStore(ClientThreadResource * res , char * request , int * result){
		
		char * fileName = strstr(request , " "); fileName++;
		res->argument = newString( res->heapHandler , fileName , strlen(fileName) );
		
		res->transferTask = STOR_TRANSFER;

		ResumeThread( res->dataThreadHandler );
		WaitForSingleObject( res->dataThreadHandler , INFINITE );

		sendMessage(res->controlSocket , "220 Transfer Complete" , result);
	}

	void executeCommandCwd(ClientThreadResource * res , int * result){
		/***
		Falta implementarla
		***/
	}

	void doDataService(ClientThreadResource * res){
		
		ThreadHandler handler = _beginthreadex(NULL, 0, &serviceDataThread, res, CREATE_SUSPENDED , NULL);
		res->dataThreadHandler = handler;
	}


	void validateAndCloseDataConnection(ClientThreadResource * res){
		/**
		 * Si actualmente se encuentra activo entonces es 
		 * necesario cerrar el socket y thread activos
		 */
		if(res->mode != FTP_DATA_MODE_INACTIVE){
			closeSocket(res->dataSocket);
			WaitForSingleObject(res->dataThreadHandler , INFINITE);
		}
	}