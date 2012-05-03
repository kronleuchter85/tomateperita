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

	void configure(){
		setupConfiguration();
		setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
	}

	void executeCommand(char * command , char * arg , ListenSocket s){
		if(isCommandFormat(command)){
			executeKssCommandFormat(arg ,s);
		}else if(isCommandMount(command)){
			executeKssCommandMount(arg,s);
		}else if(isCommandMd5Sum(command)){
			executeKssCommandMd5Sum(arg , s);
		}else if(isCommandTddDump(command)){
			executeKssCommandTddDump(s);
		}else if(isCommandUmount(command)){
			executeKssCommandUmount(arg , s);
		}else if(isCommandList(command)){
			executeKssCommandList(arg , s);
		}else if(!isCommandQuit(command)){
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

		char command[SHELL_BUFFER_SIZE];

		do{
			bzero(command , SHELL_BUFFER_SIZE);
			printf("\n");
			printf("--------------------------------------------------------------------------------------\n");
			printf("comand> ");
			//scanf("%s" , command);

			//executeCommand(command , s);

			executeCommand("mount" , "VDA1" , s);
			executeCommand("mount" , "VDA2" , s);
			executeCommand("mount" , "VDA3" , s);
			executeCommand("mount" , "VDA4" , s);
			executeCommand("mount" , "VDA5" , s);
			executeCommand("mount" , "VDA6" , s);

			executeCommand("format" , "VDA1" , s);
			executeCommand("format" , "VDA2" , s);
			executeCommand("format" , "VDA3" , s);
			executeCommand("format" , "VDA4" , s);
			executeCommand("format" , "VDA5" , s);
			executeCommand("format" , "VDA6" , s);

			executeCommand("ls" , "/" , s);
			executeCommand("ls" , "VDA1" , s);
			executeCommand("ls" , "VDA2" , s);
			executeCommand("ls" , "VDA3" , s);
			executeCommand("ls" , "VDA4" , s);
			executeCommand("ls" , "VDA5" , s);
			executeCommand("ls" , "VDA6" , s);

			executeCommand("umount" , "VDA1" , s);
			executeCommand("umount" , "VDA2" , s);
			executeCommand("umount" , "VDA3" , s);
			executeCommand("umount" , "VDA4" , s);
			executeCommand("umount" , "VDA5" , s);
			executeCommand("umount" , "VDA6" , s);

			sleep(5);
		}while( !isCommandQuit(command));

		printf("Finalizando Shell...\n\n");
		close(s);

		return EXIT_SUCCESS;
	}



