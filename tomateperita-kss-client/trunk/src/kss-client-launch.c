/*
 ============================================================================
 Name        : tomateperita-kss-client.c
 Author      : gonzalo
 Version     :
 Copyright   : Your copyright notice
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
#include "linux-commons-logging.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-remote-services.h"

#include "kss-client-configuration.h"
#include "kss-client-commands.h"
#include "kss-client-endpoint.h"
#include "kss-client-utils.h"

#define SHELL_BUFFER_SIZE 256

	void executeCommand(char * command , char * arg , ListenSocket s);


	void configure(){
		setupConfiguration();
		setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
	}

	void executeCommand(char * command , char * arg , ListenSocket s){
		if(isCommandFormat(command)){
			callKssOperationFormat(arg ,s);
		}else if(isCommandMount(command)){
			callKssOperationMount(arg,s);
		}else if(isCommandMd5Sum(command)){
			callKssOperationMd5Sum(arg , s);
		}else if(isCommandTddDump(command)){
			callKssOperationTddDump(s);
		}else if(isCommandUmount(command)){
			callKssOperationUmount(arg , s);
		}else if(isCommandList(command)){
			callKssOperationList(arg , s);
		}else if(isCommandDisconnect(command)){
			callKssOperationDisconnect(arg , s);
		}else if(isCommandShowDevices(command)){
			callKssOperationShowDevices(s);
		}else if(isCommandQuit(command)){
			callKssOperationQuit(arg , s);
		}else{
			printf("Comando invalido\n");
		}
	}


	int main(void) {

		configure();

		ListenSocket s = openClientConnection(getKssServerHost() , getKssServerPort());

		if(s == INVALID_SOCKET){
			error("no se puede abrir la coneccion");
			return EXIT_FAILURE;
		}


		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		doHandshake(s , validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return EXIT_FAILURE;
		}

		char fullCommand[SHELL_BUFFER_SIZE];

		do{
			bzero(fullCommand , SHELL_BUFFER_SIZE);
			printf("\n");
			printf("--------------------------------------------------------------------------------------\n");
			printf("kss-shell-command> ");
			fgets( fullCommand , SHELL_BUFFER_SIZE , stdin );

			List tokens = tokenize( fullCommand , ' ');
			Iterator * ite = buildIterator(tokens);
			char * command = next(ite);
			char * cmd = trim(command);

			if ( hasMoreElements(ite) ){
				char * argument = next(ite);
				char * arg = trim(argument);

				executeCommand(cmd , arg , s);
			} else {
				executeCommand(cmd , NULL , s);
			}

			//testShell();

		}while( !isCommandQuit(fullCommand));

		printf("Finalizando Shell...\n\n");
		close(s);

		return EXIT_SUCCESS;
	}



