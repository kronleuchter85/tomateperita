/**
 * ftp-microkernel.c
 *
 * Punto de entrada de la aplicacion. Es el 'Main'...
 * Tiene señales de control manipulables mediante las funciones
 * setStatus y getStatus, que modifican la variable KERNEL_STATUS.
 *
*/


#include "windows-commons.h"
#include "windows-commons-headers.h"
#include "windows-commons-socket.h"

#include "windows-commons-memory.h"
#include "windows-commons-logging.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-configuration.h"
#include "ftp-state.h"
#include "ftp-commons.h"


	void configure(){
		setProcessName(FTPS);
		setGlobalHeap( createHeap(1024) );		
		setUpConfiguration();
		setKernelStatus( KERNEL_STATUS_CONTINUE );
	}


	int main(void){				
		
		RuntimeErrorValidator * validator;

		configure();

		validator = buildErrorSuccessValidator(getGlobalHeap());

		startService(validator , getGlobalHeap());
	
		if(hasError(validator)){
			error(validator->errorDescription);
		}

		if(isInfoEnabled()) 
			info("Liberando recursos ocupados por el Server");
		
		destroyHeap(getGlobalHeap());

		testStrings();

		return EXIT_SUCCESS;
	}




