/*
 ============================================================================
 Name        : fss-microkernel.c
 Author      : 
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-logging.h"
#include "linux-commons-file.h"
#include "linux-errors.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"

#include "fss-configuration.h"
#include "fss-state.h"
#include "fss-utils.h"
#include "fss-fs-management.h"
#include "fss-mps-entrypoint.h"


	void setupDirectoryIndexes(){
		if(existsFile(getDataDirectory())){
			if(isInfoEnabled()) info(concatAll( 3, "Usando " , getDataDirectory() , " como directorio de indices"));
		}else{
			if(isInfoEnabled()) info(concatAll(2 , "No existe el directorio " , getDataDirectory()));

			if(createDirectory(getDataDirectory())){
				if(isInfoEnabled()) info(concatAll(2 , "Se crea " , getDataDirectory()));
			}else{
				if(isInfoEnabled()) info(concatAll(2 , "No se pudo crear el directorio " , getDataDirectory()));
				killAndWait(1);
				return;
			}
		}
	}

	void configure(){
		setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
		setupConfiguration();
		setFssRunningStatus(TRUE);
		setupDirectoryIndexes();
	}


	int main(void) {

		configure();

		//testFormat();
		//testCreate();
		//testCreatingBitVector();
		//testUpdateSize();
		//testGettingFileInformation();
		//testAddingSectors();
		//testListingFiles();
		//testFreeingSectors();
		//testGetCurrentSize();

		//testListImplementation();
		//testListImplementation2();

		if(isInfoEnabled())
			info("Conectando con modulo KSS");

		ListenSocket s = openClientConnection(getKssHost() , getKssPort());

		if(s == INVALID_SOCKET){
			error("no se puede abrir la coneccion");
			return EXIT_FAILURE;
		}

		if(isInfoEnabled())
			info("Quedando a la espera de peticiones para realizar operaciones FSS");

		RuntimeErrorValidator * validator ;
		MpsRequest * request = NULL;

		while(isFssRunningStatus()){
			validator = buildErrorSuccessValidator();
			request = receiveMpsRequest(s , validator);
			validator = buildErrorSuccessValidator();
			info("Reciviendo peticion MPS");

			if(isInfoEnabled())
				info(concatAll(3 , "Header.DescriptorId: '" , strndup(request->descriptorId , 16) , "'"));

			if(isInfoEnabled())
				info( concatAll(3 , "Payload.Arguments: '" , formatListToPreetyString(request->arguments) , "'"));

			if(isInfoEnabled())
				info(concatAll(3 , "Payload.OperationName: '" , request->operationName , "'"));

			executeOperation(s , request , validator);
		}

		return EXIT_SUCCESS;
	}




