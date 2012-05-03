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
#include "kss-client-configuration.h"
#include "kss-client-commands.h"

#define SHELL_BUFFER_SIZE 256

	void configure(){
		setupConfiguration();
	}

	BOOL isCommandQuit(char * c){
		return startsWith(SHELL_COMMAND_QUIT , c);
	}


	int main(void) {

		configure();

		ListenSocket s = openClientConnection(getKssServerHost() , getKssServerPort());
		char command[SHELL_BUFFER_SIZE];

		do{
			bzero(command , SHELL_BUFFER_SIZE);
			printf("comand> ");
			scanf("%s" , command);

			sendMessage(s , command );
		}while( !isCommandQuit(command));

		close(s);

		return EXIT_SUCCESS;
	}
