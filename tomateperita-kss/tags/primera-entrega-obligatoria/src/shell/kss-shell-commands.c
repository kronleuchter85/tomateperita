/*
 * kss-shell-commands.c
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <stdio.h>

#include "kss-shell-service.h"
#include "linux-commons.h"
#include "kss-state.h"

	void executeCommand(char * cmd){

		if(isCommandMount(cmd)){

			executeCommandMount(cmd);
		}else if(isCommandUmount(cmd)){

			executeCommandUmount(cmd);
		}else if(isCommandFormat(cmd)){

			executeCommandFormat(cmd);
		}else if(isCommandList(cmd)){

			executeCommandList(cmd);
		}else if(isCommandMd5Sum(cmd)){

			executeCommandMd5Sum(cmd);
		}else if(isCommandTddDump(cmd)){

			executeCommandTddDump(cmd);
		}else if(isCommandShutdown(cmd)){

			executeCommandShutdown();
		}
	}

	void executeCommandMount(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandUmount(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandFormat(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandTddDump(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandMd5Sum(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandList(char * cmd){
		//TODO: loguear como corresponde
		printf("operacion no implementada por ausencia del modulo FSS");
	}
	void executeCommandShutdown(){
		finishKernel();
	}

	BOOL isCommandMount(char * cmd){
		return startsWith(SHELL_COMMAND_MOUNT , cmd);
	}
	BOOL isCommandUmount(char * cmd){
		return startsWith(SHELL_COMMAND_UMOUNT , cmd);
	}
	BOOL isCommandFormat(char * cmd){
		return startsWith(SHELL_COMMAND_FORMAT , cmd);
	}
	BOOL isCommandList(char * cmd){
		return startsWith(SHELL_COMMAND_LIST, cmd);
	}
	BOOL isCommandTddDump(char * cmd){
		return startsWith(SHELL_COMMAND_TDD_DUMP, cmd);
	}
	BOOL isCommandMd5Sum(char * cmd){
		return startsWith(SHELL_COMMAND_MD5SUM , cmd);
	}
	BOOL isCommandQuit(char * cmd){
		return startsWith(SHELL_COMMAND_QUIT , cmd);
	}
	BOOL isCommandShutdown(char * cmd){
		return startsWith(SHELL_COMMAND_SHUTDOWN , cmd);
	}
