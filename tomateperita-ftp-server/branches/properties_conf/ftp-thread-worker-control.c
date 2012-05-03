/**
 * ftp-thread-worker.c
 *
 * Hilo creado para la atencion de cada peticion 
 * de clientes conectados.
 */

#include "windows-commons.h"
#include "windows-commons-headers.h"
#include "windows-commons-socket.h"
#include "windows-commons-memory.h"
#include "windows-commons-logging.h"


#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp-configuration.h"
#include "ftp.h"



	void executeCommandPwd(ClientThreadResource *  );
	void executeCommandUser(ClientThreadResource *  );
	void executeCommandSyst(ClientThreadResource *   );	
	void executeCommandType(ClientThreadResource *  , char *  );
	void executeCommandPasv(ClientThreadResource *   );	
	void executeCommandList(ClientThreadResource *);
	void executeCommandNotAllowed(ClientThreadResource * );
	void executeCommandStore(ClientThreadResource *, char * );
	void executeCommandRetrieve(ClientThreadResource *, char * );
	void executeCommandCwd(ClientThreadResource * , char *);
	void executeCommandNoop(ClientThreadResource * );



	/**
	 * Funcion que representa la ejecucion del hilo...
	 * Establecido el control connection entre el server-pi y el 
	 * user-pi se atienden los comandos del usuario hasta que decida
	 * terminar la coneccion.
	 */
	unsigned __stdcall serviceControlThread( void* pArguments ){
		
		char receiving[1024];
		int received = 0;
		
		ClientThreadResource * ctr = (ClientThreadResource *) pArguments;

		ctr->relativeWorkingDir = "\"/\"";

		sendBytes(ctr->controlSocket , buildResponseMessageType220(ctr->heapHandler) , 
			strlen(buildResponseMessageType220(ctr->heapHandler)));

		do{
			ZeroMemory(receiving , 1024);
			received = recv(ctr->controlSocket , receiving , 1024 , 0);

			if (received > 0) 
				execute(trimAll(receiving , ctr->heapHandler) , ctr  );

		}while( !startsWith(receiving , COMMAND_QUIT) && (received > 0));

		if(isDebugEnabled())
			debug( concat(ctr->heapHandler , 2 , "Cerrando socket remoto SocketId=" , 
				intToString(ctr->controlSocket , ctr->heapHandler)) );	

		closeSocket(ctr->controlSocket);				

		return 0;
	}





	

	void execute(char * aRequest , ClientThreadResource * aResource ){
						
		if( startsWith(aRequest , COMMAND_USER)){

			if(isAnonymousAccess()){
				executeCommandUser( aResource );
			}

		}else if(startsWith(aRequest, COMMAND_TYPE)){

			executeCommandType(aResource , "I" );

		}else if( startsWith(aRequest , COMMAND_PWD)){

			executeCommandPwd( aResource );

		}else if( startsWith(aRequest , COMMAND_LIST)){

			executeCommandList(aResource );

		}else if( startsWith(aRequest , COMMAND_RETRIEVE)){

			executeCommandRetrieve(aResource , aRequest );

		}else if( startsWith(aRequest , COMMAND_STORE)){

			executeCommandStore(aResource, aRequest);		

		}else if(startsWith(aRequest, COMMAND_SYST)){

			executeCommandSyst(aResource );

		}else if(startsWith(aRequest , COMMAND_PASSIVE)){

			executeCommandPasv(aResource );				

		}else if(startsWith(aRequest , COMMAND_CWD)){

			executeCommandCwd(aResource , aRequest);

		}else if(startsWith(aRequest , COMMAND_NOOP)){

			executeCommandNoop(aResource);

		}else if(!startsWith(aRequest , COMMAND_QUIT)){

			executeCommandNotAllowed(aResource );
		}
	}



	void executeCommandPasv(ClientThreadResource * res ){		
		int cantRetries = 0;
		char * port = calculatePort(getServerDataPort() , res->heapHandler);
		res->dataSocket = openServerSocket(getServerAddress() , port);

		while( (res->dataSocket == INVALID_SOCKET)
			&& (cantRetries < getMaxRetriesFindingDataPort())){
				port = calculatePort(port , res->heapHandler);
				res->dataSocket = openServerSocket(getServerAddress() , port);
				cantRetries++;
		}
		res->dtpPort = port;				
		sendBytes(res->controlSocket , buildResponseMessageType227(res->heapHandler , port) , 
			strlen(buildResponseMessageType227(res->heapHandler , port)));
	}


	
	void executeCommandStore(ClientThreadResource * res , char * request ){
		char * fileName = replaceAll(request , COMMAND_STORE , "");
		fileName = trimAll(fileName , res->heapHandler);
		res->argument = getPhysicalFilePath(res , fileName);
		res->transferTask = STOR_TRANSFER;
		sendBytes(res->controlSocket , buildResponseMessageType150(res->heapHandler) , 
			strlen(buildResponseMessageType150(res->heapHandler)));
		doDataService(res);
		blockMutex( res->dataThreadHandler );
		sendBytes(res->controlSocket , res->resultMessage , strlen(res->resultMessage));
	}




	void executeCommandList(ClientThreadResource * res ){
		res->transferTask = LIST_TRANSFER;
		sendBytes(res->controlSocket ,  buildResponseMessageType150(res->heapHandler) , 
			strlen( buildResponseMessageType150(res->heapHandler)));
		doDataService(res);	
		blockMutex(res->dataThreadHandler);	
		sendBytes(res->controlSocket , res->resultMessage , strlen(res->resultMessage));
	}



	

	void executeCommandRetrieve(ClientThreadResource * res , char * request ){		
		char * path = trimAll(replaceAll(request , COMMAND_RETRIEVE , "") , res->heapHandler);	
		res->transferTask = RETR_TRANSFER;
		res->argument = getPhysicalFilePath(res , path);
		//res->argument = concat( res->heapHandler , 3 , getDataDirectory() , "/" , path) ;		
		
		sendBytes(res->controlSocket , buildResponseMessageType150(res->heapHandler), 
			strlen(buildResponseMessageType150(res->heapHandler)));
		doDataService(res);
		blockMutex( res->dataThreadHandler );		
		sendBytes(res->controlSocket , res->resultMessage , strlen(res->resultMessage));
	}




	void executeCommandUser(ClientThreadResource * res ){		
		sendBytes(res->controlSocket ,  buildResponseMessageType230(res->heapHandler), 
			strlen( buildResponseMessageType230(res->heapHandler)));
	}


	
	void executeCommandPwd(ClientThreadResource * aRes ){		
		char * path = concat(aRes->heapHandler , 3, "\"" , aRes->relativeWorkingDir , "\"");
		char * response = buildResponseMessageType257(aRes->heapHandler , path);
		sendBytes(aRes->controlSocket , response , strlen(response));
	}





	void executeCommandSyst(ClientThreadResource * res ){
		sendBytes(res->controlSocket , buildResponseMessageType215(res->heapHandler) , 
			strlen(buildResponseMessageType215(res->heapHandler)));
	}




	void executeCommandNotAllowed(ClientThreadResource * res ){		
		int result;
		sendBytes(res->controlSocket , buildResponseMessageType500(res->heapHandler) , 
			strlen(buildResponseMessageType500(res->heapHandler)));
	}





	void executeCommandType(ClientThreadResource * res , char * type ){	
		sendBytes(res->controlSocket , buildResponseMessageType200(res->heapHandler) , 
			strlen(buildResponseMessageType200(res->heapHandler)));
	}
	


	void executeCommandCwd(ClientThreadResource * res , char * path ){	
		path = replaceAll(path , COMMAND_CWD , "");
		path = trimAll(path , res->heapHandler);
		res->relativeWorkingDir = path;
		sendBytes(res->controlSocket , buildResponseMessageType250( res->heapHandler) , 
			strlen(buildResponseMessageType250( res->heapHandler)));
	}




	void executeCommandNoop(ClientThreadResource * res ){
		sendBytes(res->controlSocket , buildResponseMessageType200(res->heapHandler) , 
			strlen(buildResponseMessageType200(res->heapHandler)));
	}


