/*
 * kss-shell-service.h
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */

#include "linux-commons.h"

#ifndef KSS_SHELL_SERVICE_H_
#define KSS_SHELL_SERVICE_H_

/**
 *  Comandos pedidos por la catedra
 */
#define SHELL_COMMAND_MOUNT 	"mount"
#define SHELL_COMMAND_UMOUNT 	"umount"
#define SHELL_COMMAND_LIST		"ls"
#define SHELL_COMMAND_FORMAT	"format"
#define SHELL_COMMAND_TDD_DUMP	"tdd_dump"
#define SHELL_COMMAND_MD5SUM	"md5sum"

/**
 * Comandos adicionales
 */
#define SHELL_COMMAND_QUIT		"quit"
#define SHELL_COMMAND_SHUTDOWN	"shutdown"

	void * runServiceShellThread(void *);
	void * runShellServiceThread(void * );

	BOOL isCommandMount(char * );
	BOOL isCommandUmount(char * );
	BOOL isCommandFormat(char * );
	BOOL isCommandList(char * );
	BOOL isCommandTddDump(char * );
	BOOL isCommandMd5Sum(char * );
	BOOL isCommandQuit(char * );
	BOOL isCommandShutdown(char *);

	void executeCommand(char *);
	void executeCommandMount(char *);
	void executeCommandUmount(char *);
	void executeCommandFormat(char *);
	void executeCommandTddDump(char *);
	void executeCommandMd5Sum(char *);
	void executeCommandList(char *);
	void executeCommandShutdown();

#endif /* KSS_FSS_SERVICE_H_ */
