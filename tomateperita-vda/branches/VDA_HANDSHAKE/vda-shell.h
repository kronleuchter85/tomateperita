/**
 *  Funciones para la consola de comandos
 */

#include "vda-shell-parser.h"


#ifndef _W_VDA_SHELL_H_
#define _W_VDA_SHELL_H_


#define CMD_PARAM_SEPARATOR		","
#define CMD_OPEN				"("
#define CMD_CLOSE				")"
#define CMD_EOL					10
#define CMD_EOS					'\0'
#define CMD_MAX_LEN				30
#define CMD_MAX_ARG				(MAX_SYMBOLS_COUNT -1)
#define CMD_ARG_MAX_LEN			8
#define CMD_OK					0
#define CMD_ERR1				1
#define CMD_ERR2				2
#define CMD_ERR3				3

#define SHELL_COMMAND_QUIT		"quit"



	typedef struct {
        char commandName[CMD_MAX_LEN];
		char args[CMD_MAX_ARG][CMD_MAX_LEN];
        int argsCount;
       } ShellCommand;

	ShellCommand * createShellCommand(char * commandLine);

	char * getCommandName();
	char * getArgument(int i);
	int getArgumentsCount();
	

#endif