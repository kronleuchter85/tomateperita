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

	void executeCommandMount(ListenSocket l , MpsMessage * req);
	void executeCommandUmount(ListenSocket l , MpsMessage * req);
	void executeCommandFormat(ListenSocket l , MpsMessage * req);
	void executeCommandTddDump(ListenSocket l , MpsMessage * req);
	void executeCommandMd5Sum(ListenSocket l , MpsMessage * req);
	void executeCommandList(ListenSocket l , MpsMessage * req);
	void executeCommandQuit(ListenSocket l , MpsMessage * req);

#endif /* KSS_FSS_SERVICE_H_ */
