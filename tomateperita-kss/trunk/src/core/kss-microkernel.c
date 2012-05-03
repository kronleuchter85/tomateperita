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



	Thread fssServiceThread;
	Thread ftpServiceThread;
	Thread shellServiceThread;
	Thread vdaServiceThread;


	void closeKss(){

		if(isInfoEnabled())
			info("cerrando socket de file system");
		close(getFileSystemConnection());
		close(getFssServerSocket());

		if(isInfoEnabled())
			info("cerrando socket de Ftp");
		close(getFtpServerSocket());

		if(isInfoEnabled())
			info("cerrando socket de vda");
		close(getVdaServerSocket());

		if(isInfoEnabled())
			info("cerrando socket de Shell");
		close(getShellServerSocket());
		exit(0);
	}


	void closingEvents(){
		signal(SIGTERM , closeKss);
		signal(SIGKILL , closeKss);
		signal(SIGQUIT , closeKss);
		signal(SIGINT , closeKss);
	}


	void configure(RuntimeErrorValidator * validator){
		closingEvents();
		setKssRunningStatus(TRUE);
		//setLoggingLevelEnabled(LOGGING_LEVEL_DEBUG);
		setupConfiguration(validator);

		/**
		 * inicializamos la tdd
		 */
		initializeTdd();

		if(hasError(validator)){
			return;
		}
	}




	void launch(){


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
			return ;
		}

		if( pthread_create(&shellServiceThread , NULL , runShellServiceThread , NULL) ){
			error("termino con error");
			return ;
		}

		if( pthread_create(&ftpServiceThread , NULL , runFtpServiceThread , NULL) ){
			error("termino con error");
			return ;
		}

	}


	void waitForThreads(){

		pthread_join(ftpServiceThread , NULL);
		pthread_join(shellServiceThread, NULL);
		pthread_join(vdaServiceThread, NULL);
		pthread_join(fssServiceThread, NULL);
	}


	void launchTests(){
		do{
			sleep(2);

			info("chequeando conecciones fss");
			if(hasAvailableFss()){
				info("se encontro coneccion, mandando mensaje y haciendo pruebas");
				integrateFss();
				//testFilesTransferMps2();
			}else{
				info("no hay coonecciones activas");
			}


			info("chequeando conecciones fss");
			if(hasAvailableVdas()){
				info("se encontro coneccion, mandando mensaje y haciendo pruebas");
				//integrateVda();
				integrateVdasWIthBlocks();
			}else{
				info("no hay coonecciones activas");
			}


			if(isKssOperationalStatus()){
				info("se encontro coneccion, mandando mensaje y haciendo pruebas");
				integrateFtps();
			}else{
				info("no hay conecciones activas");
			}

			//populateAndAddTddRecords();


		}while( isKssRunningStatus() );
	}


	int main(void) {

		/**
		 * Levanta la configuracion del archivo dejandola disponible
		 * en las correspondientes variables.
		 *
		 */

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		configure(validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return EXIT_FAILURE ;
		}


		launch();

		waitForThreads();

		//launchTests();

		info("terminado el microkernel");

		return EXIT_SUCCESS;
	}



