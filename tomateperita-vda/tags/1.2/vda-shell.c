/**
 *  Funciones para la consola de comandos
 */

#include "vda-shell.h"
#include "vda-shell-parser.h"
#include "vda-state.h"
#include <stdio.h>
#include <string.h>


	char * getCommandName(){
		return getSymbol(0);
	}	
	char * getArgument(int i){
		return getSymbol( i +1);
	}
	int getArgumentsCount(){
		return getSymbolsCount() -1;
	}


	ShellCommand * createShellCommand(char * commandLine){

		int i;
		ShellCommand * command = allocateMemory(getGlobalHeap() , sizeof(ShellCommand));

		//parseamos el commandLine y creamos la matriz de symbolos
		parseSequence(commandLine);

		//seteamos la cantidad de argumentos
		command->argsCount = getArgumentsCount();
		
		//ponemos en 0 todos los caracteres del comando
		memset(command->commandName , '\0' , CMD_MAX_LEN);
		//ponemos en 0 todos los caracteres de todos los argumentos
		for(i=0 ; i<CMD_MAX_ARG ; i++)
			memset(command->args[i] , '\0' , CMD_MAX_LEN);

		//seteamos el comando
		strcpy_s(command->commandName, CMD_MAX_LEN, getCommandName());
		//strcpy(command->commandName , getCommandName());
		//seteamos todos los argumentos
		for( i=0 ; i<getArgumentsCount() ; i++){
			strcpy_s(command->args[i] , CMD_ARG_MAX_LEN, getArgument(i));
			//strcpy(command->args[i] , getArgument(i));
		}

		return command;	
	}
