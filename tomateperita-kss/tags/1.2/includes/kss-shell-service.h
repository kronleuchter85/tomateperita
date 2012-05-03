/*
 * kss-shell-service.h
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */

#include "linux-commons.h"
#include "linux-commons-mps.h"

#ifndef KSS_SHELL_SERVICE_H_
#define KSS_SHELL_SERVICE_H_


/**
 * Comandos adicionales
 */

	void * runServiceShellThread(void *);
	void * runShellServiceThread(void * );

	Bool isCommandQuit(char * );

	void executeCommand(ListenSocket l , MpsRequest * req);
	void executeCommandMount(ListenSocket l , MpsRequest * req);
	void executeCommandUmount(ListenSocket l , MpsRequest * req);
	void executeCommandFormat(ListenSocket l , MpsRequest * req);
	void executeCommandTddDump(ListenSocket l , MpsRequest * req);
	void executeCommandMd5Sum(ListenSocket l , MpsRequest * req);
	void executeCommandList(ListenSocket l , MpsRequest * req);
	void executeCommandShutdown(ListenSocket l , MpsRequest * req);

#endif /* KSS_FSS_SERVICE_H_ */
