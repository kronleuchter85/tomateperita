/*
 * kss-client-commands.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */


#include "linux-commons.h"
#include "kss-client-commands.h"

	Bool isCommandMount(char * cmd){
		return equalsStrings(trim(cmd) , SHELL_COMMAND_MOUNT );
	}
	Bool isCommandUmount(char * cmd){
		return equalsStrings(trim(cmd)  , SHELL_COMMAND_UMOUNT );
	}
	Bool isCommandFormat(char * cmd){
		return equalsStrings(SHELL_COMMAND_FORMAT , trim(cmd) );
	}
	Bool isCommandList(char * cmd){
		return equalsStrings(SHELL_COMMAND_LIST, trim(cmd) );
	}
	Bool isCommandTddDump(char * cmd){
		return equalsStrings(SHELL_COMMAND_TDD_DUMP, trim(cmd) );
	}
	Bool isCommandMd5Sum(char * cmd){
		return equalsStrings(SHELL_COMMAND_MD5SUM , trim(cmd) );
	}
	Bool isCommandQuit(char * cmd){
		return equalsStrings(SHELL_COMMAND_QUIT , trim(cmd) );
	}


