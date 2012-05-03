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
	void executeCommandNotAllowed(ClientThreadResource *, int * );
	void executeCommandStore(ClientThreadResource *, char * , int *);
	void executeCommandRetrieve(ClientThreadResource *, char * , int *);
	void executeCommandCwd(ClientThreadResource * , int *);
	void executeCommandNoop(ClientThreadResource * , int *);


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
				executeCommandRetrieve(aResource , aRequest , iResult);
			}else if( startsWith(aRequest , COMMAND_STORE)){
				executeCommandStore(aResource, aRequest , iResult);			
			}else if(startsWith(aRequest, COMMAND_SYST)){
				executeCommandSyst(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_PASSIVE)){
				executeCommandPasv(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_PORT)){
				executeCommandPort(aResource , aRequest , iResult);
			}else if(startsWith(aRequest , COMMAND_OPTS)){
				executeCommandNotAllowed(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_SIZE)){
				executeCommandNotAllowed(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_CWD)){
				executeCommandCwd(aResource , iResult);
			}else if(startsWith(aRequest , COMMAND_NOOP)){
				executeCommandNoop(aResource , iResult);
			}else if(!startsWith(aRequest , COMMAND_QUIT)){
				executeCommandNotImplemented(aResource , iResult);
			}
	}

	void executeCommandUser(ClientThreadResource * aResource , int * iResult){
		char * message = concat(aResource->heapHandler , 3 , RESPONSE_CODE_230 ,  
			" usuario conectado " , CHARACTER_CRLF);
		debug(trim(concat(aResource->heapHandler , 2 , "Respuesta: " , message),aResource->heapHandler));
		sendMessage(aResource->controlSocket , message , &iResult);
	}

	void executeCommandPwd(ClientThreadResource * aRes , char * workingDir , int * iResult){
		char * message = concat(aRes->heapHandler , 4 , RESPONSE_CODE_257 , 
			CHARACTER_SP , "\"/\" is the current directory" , CHARACTER_CRLF);
		debug(trim(concat(aRes->heapHandler , 2 , "Respuesta: " , message),aRes->heapHandler));
		sendMessage(aRes->controlSocket , message , &iResult);
	}

	void executeCommandSyst(ClientThreadResource * res , int * iresult){
		char * m = concat(res->heapHandler , 2 , "215 Windows XP " , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &iresult);
	}

	void executeCommandNotImplemented(ClientThreadResource * res , int * result){
		char * m = concat(res->heapHandler , 2 , 
			"502 Orden no implementada, no necesaria en este sistema " , CHARACTER_CRLF);
		debug(trim(concat(res->heapHandler , 2 , "Respuesta: " , m),res->heapHandler));
		sendMessage(res->controlSocket , m , &result);
	}

	void executeCommandNotAllowed(ClientThreadResource * res , int * result){
		char * m = concat(res->heapHandler , 2 ,
			"500 Comando no permitido" , CHARACTER_CRLF);
		debug(trim(concat(res->heapHandler , 2 , "Respuesta: " , m),res->heapHandler));
		sendMessage(res->controlSocket , m , &result);
	}

	void executeCommandType(ClientThreadResource * res , char * type , int * result){
		char * m = concat(res->heapHandler , 2 , "200 Type accepted " , CHARACTER_CRLF);
		debug(trim(concat(res->heapHandler , 2 , "Respuesta: " , m),res->heapHandler));
		sendMessage(res->controlSocket , m , &result);
	}
	
	void executeCommandPasv(ClientThreadResource * res , int * result){
		char * m;
		int cantRetries = 0;
		char * port = calculatePort(getServerDataPort() , res->heapHandler);
		
		res->dataSocket = openServerSocket(getServerAddress() , port);
		while( (res->dataSocket == INVALID_SOCKET)
			&& (cantRetries < getMaxRetriesFindingDataPort())){
				port = calculatePort(port , res->heapHandler);
				res->dataSocket = openServerSocket(getServerAddress() , port);
				cantRetries++;
		}

		validateAndCloseDataConnection(res);
		res->mode = FTP_DATA_MODE_PASIVE;
		
		m = concat(res->heapHandler , 4 , 
			"227 (" , encodeHostAndPort(getServerAddress() , port , res->heapHandler), 
			")" , CHARACTER_CRLF);
		
		sendMessage(res->controlSocket , m , &result);
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
		sendMessage(res->controlSocket , concat(res->heapHandler , 2 , 
			"150 Iniciando transferencia de datos " , CHARACTER_CRLF) , &result);

		unblockMutex(res->dataMutex);		
		blockMutex(res->controlMutex);

		res->argument = getDataDirectory();
		res->transferTask = LIST_TRANSFER;
		
		doDataService(res);

		WaitForSingleObject( res->dataThreadHandler , INFINITE );
		
		debug(res->resultMessage);
		sendMessage(res->controlSocket , res->resultMessage , &result);
	}

	void executeCommandRetrieve(ClientThreadResource * res , char * request , int * result){
		char * m;
		request = strstr( request , " "); request++;
		res->argument = trimAll(concat(res->heapHandler , 3 , 
			getDataDirectory() , "/" , request) ,
			res->heapHandler);
		
		m = concat(res->heapHandler , 3 , "150 Opening data tansfer ", 
			res->argument, CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &result);
		debug(concat(res->heapHandler , 2 , "Respuesta " , m)); //esto se puede ir despues

		res->transferTask = RETR_TRANSFER;

		doDataService(res);

		WaitForSingleObject( res->dataThreadHandler , INFINITE );

		debug(res->resultMessage);
		sendMessage(res->controlSocket , res->resultMessage , &result);
	}

	void executeCommandStore(ClientThreadResource * res , char * request , int * result){
		char * m;
		request = strstr( request , " "); request++;
		res->argument = trimAll(concat(res->heapHandler , 3 , 
			getDataDirectory() , "/" , request) ,
			res->heapHandler);

		m = concat(res->heapHandler , 3 , "150 Opening data transfer ",
			res->argument , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &result);
		debug(concat(res->heapHandler , 2 , "Respuesta: " , m));  //esto se puede ir despues

		res->transferTask = STOR_TRANSFER;

		doDataService(res);

		WaitForSingleObject( res->dataThreadHandler , INFINITE );

		debug(res->resultMessage);
		sendMessage(res->controlSocket , res->resultMessage , &result);
	}

	void executeCommandCwd(ClientThreadResource * res , int * result){
		char * m = concat(res->heapHandler , 4 , "250" , CHARACTER_SP , "CWD Command successul" , CHARACTER_CRLF);
		sendMessage(res->controlSocket , m , &result);
	}

	void executeCommandNoop(ClientThreadResource * res , int * result){
		sendMessage(res->controlSocket , concat(res->heapHandler , 2 ,
			"200 Noop command successful" , CHARACTER_CRLF) ,
			&result);
	}

	void doDataService(ClientThreadResource * res){
		
		ThreadHandler handler = _beginthreadex(NULL, 0, &serviceDataThread, res, 0 , NULL);
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