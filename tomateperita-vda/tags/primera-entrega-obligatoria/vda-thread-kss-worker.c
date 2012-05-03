
#include "socket-commons.h"
#include "commons.h"


	unsigned __stdcall doKssService( void* pArguments ){

		int error;
		char * message;

		if(isDebugEnabled()) debug( "Atendiendo peticiones Kss");

		do{
			message = receiveMessage( getKssConnection() , getGlobalHeap() , &error);

			/*
			 * recibir mensaje MPS y procesarlo
			 */
		}while( isContinueKssThread() );

		/*
		 * liberar recursos ocupados antes de finalizar
		 * el thread
		 */
		return EXIT_SUCCESS;
	}
	

		