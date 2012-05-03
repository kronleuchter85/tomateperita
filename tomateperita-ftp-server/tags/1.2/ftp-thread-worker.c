/**
 * ftp-thread-worker.c
 *
 * Hilo creado para la atencion de cada peticion 
 * de clientes conectados.
 */

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
#include "list-commons.h"
#include "ftp-commons.h"
#include "ftp-configuration.h"

	//TODO: ver si se puede implementar esto con getters y setters
	extern SortedList clientResources;

	/**
	 * Funcion que representa la ejecucion del hilo...
	 * Establecido el control connection entre el server-pi y el 
	 * user-pi se atienden los comandos del usuario hasta que decida
	 * terminar la coneccion.
	 */
	unsigned __stdcall serviceControlThread( void* pArguments ){
			
		char * workPath = getDataDirectory();
		char * clientDtpPort = NULL;

		char * receiving;
		int iResult;

		ClientThreadResource * ctr = (ClientThreadResource *) pArguments;
		
		/**
		 * Primero enviamos un 220 como saludo para indicar la coneccion 
		 * exitosa por parte de un cliente. La linea termina con un CRLF.
		 */
		sendMessage(ctr->controlSocket , concat(ctr->heapHandler , 3 , 
			RESPONSE_CODE_220 , " esperando ordenes " ,  CHARACTER_CRLF ) , &iResult);

		do{
			receiving = receiveMessage(ctr->controlSocket , ctr->heapHandler , &iResult);

			if (iResult == SOCKET_COMM_NO_ERROR) {
					if(isDebugEnabled()) {
					debug( concat(ctr->heapHandler , 2 , 
						"Se recibe un mensaje en el socket remoto SocketId=" , 
						intToString(ctr->controlSocket , ctr->heapHandler)) );
					debug(concat(ctr->heapHandler , 2 , "Mensaje: " , 
						trimNewLine(receiving , ctr->heapHandler)) );
					}
					execute(receiving , ctr , workPath , clientDtpPort , &iResult);
			}
			else
				//TODO: analizar cual es la medida que se debe tomar en base al error causado
				//TODO: implementar codigos de error
				//TODO: a travez de un archivo de errores personalizados
				//TODO: ver lista: http://msdn.microsoft.com/en-us/library/ms740668(v=vs.85).aspx				
				//TODO: analizar cual es la medida que se debe tomar en base al error causado
				//printf("[ERROR: Se produjo un error %d - %s ]\n" , WSAGetLastError() , getErrorMessage());
				error(concat(ctr->heapHandler , 3 , "Se produjo un error " , 
						intToString(WSAGetLastError() , ctr->heapHandler)));

		}while (!startsWith(receiving, COMMAND_QUIT) && (iResult == SOCKET_COMM_NO_ERROR));


		if(isDebugEnabled())
			debug( concat(ctr->heapHandler , 2 , 
				"Cerrando socket remoto SocketId=" , 
				intToString(ctr->controlSocket , ctr->heapHandler)) );
		
		removeElement(&clientResources , ctr->controlSocket , ctr->heapHandler , &disposeClientThreadResource);
		
		return 0;
	}

