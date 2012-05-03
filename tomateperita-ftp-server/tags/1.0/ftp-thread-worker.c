/**
 * ftp-thread-worker.c
 *
 * Hilo creado para la atencion de cada peticion 
 * de clientes conectados.
 */


#include "ftp-thread-worker.h"
#include "commons.h"
#include "ftp.h"


#define KILL_SERVER "KILL_SERVER"

	extern SortedList clientResources;

	/**
	 * Funcion que representa la ejecucion del hilo...
	 */
	unsigned __stdcall serviceClientThread( void* pArguments ){
			
		int iResult;

		ClientThreadResource * ctr = (ClientThreadResource *) pArguments;
		
		char * receiving = doReceive(ctr->socket , ctr->heapHandler , &iResult);
		
		if(iResult == SOCKET_COMM_ERROR){
			
			//TODO: implementar codigos de error
			//TODO: a travez de un archivo de errores personalizados
			//TODO: ver lista: http://msdn.microsoft.com/en-us/library/ms740668(v=vs.85).aspx
			printf("[ERROR: Se produjo un error %d - %s ]\n" , WSAGetLastError() , getErrorMessage());

		}else if(receiving != NULL){

			if(equalsStrings(receiving , KILL_SERVER)){
				setKernelStatus(KERNEL_STATUS_INTERRUPT_BY_INTERNAL_REQUEST);
			}					

			printf("[LOG: Se recibe un nuevo mensaje en el SocketId=%i ]\n" , ctr->socket);
			printf("[LOG: Mensaje: %s ]\n" , receiving);

			freeMemory(ctr->heapHandler , receiving);
		}

		printf("[LOG: Cerrando socket remoto SocketId=%i ]\n" , ctr->socket);

		removeElement(&clientResources , ctr->socket , ctr->heapHandler , &disposeClientThreadResource);
		
		return 0;		
	}