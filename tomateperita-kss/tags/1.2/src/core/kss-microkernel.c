/*
 ============================================================================
 Name        : tomateperita-kss.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Tomateperita KSS Microkernel, Ansi-style
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <unistd.h>

#include "linux-commons-socket.h"
#include "kss-vda-service.h"
#include "kss-ftps-service.h"
#include "kss-fss-service.h"
#include "kss-shell-service.h"
#include "kss-configuration.h"
#include "kss-state.h"
#include "kss-tdd.h"
#include "kss-utils.h"
#include "linux-commons-logging.h"


	void configure(){

		setupConfiguration();
		setKssRunningStatus(TRUE);
		setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
	}


	int main(void) {

		Thread fssServiceThread;
		Thread ftpServiceThread;
		Thread shellServiceThread;
		Thread vdaServiceThread;

		/**
		 * Levanta la configuracion del archivo dejandola disponible
		 * en las correspondientes variables.
		 *
		 */
		configure();

		/**
		 * inicializamos la tdd
		 */
		initializeTdd();

		/**
		 * LEvanta un thread para atender conecciones FSS escuchando en un socket.
		 * Cuando existe una nueva cambia el estado del flag correspondiente
		 * para indicar la existencia de la misma.
		 */
		int status = pthread_create(&fssServiceThread , NULL , runFssServiceThread , NULL);

		/**
		 *
		 * Abre el socket de coneccion VDA y espera hasta
		 * que existe una coneccion.
		 * Cuando la hay levanta un nuevo thread para atenderla e
		 * indica mediante el flag de estado que existe dicha coneccion
		 */
		status = status & pthread_create(&vdaServiceThread , NULL , runVdaServiceThread , NULL);

		if(status){
			return EXIT_FAILURE;
		}

		if( pthread_create(&shellServiceThread , NULL , runShellServiceThread , NULL) ){
			error("termino con error");
			return EXIT_FAILURE;
		}

		if( pthread_create(&ftpServiceThread , NULL , runFtpServiceThread , NULL) ){
			error("termino con error");
			return EXIT_FAILURE;
		}

		pthread_join(ftpServiceThread , NULL);
		pthread_join(shellServiceThread, NULL);
		pthread_join(vdaServiceThread, NULL);
		pthread_join(fssServiceThread, NULL);


		do{
			sleep(5);
/*
			info("chequeando conecciones fss");
			if(hasAvailableFss()){
				info("se encontro coneccion, mandando mensaje y haciendo pruebas");
				integrateFss();
			}else{
				info("no hay coonecciones activas");
			}


			info("chequeando conecciones fss");
			if(hasAvailableVdas()){
				info("se encontro coneccion, mandando mensaje y haciendo pruebas");
				integrateVda();
			}else{
				info("no hay coonecciones activas");
			}
*/
		}while( isKssRunningStatus() );

		info("terminado el microkernel");

		return EXIT_SUCCESS;
	}



