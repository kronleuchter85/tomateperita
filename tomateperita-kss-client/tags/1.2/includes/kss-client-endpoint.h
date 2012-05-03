/*
 * kss-client-commands.h
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"

#ifndef KSS_CLIENT_ENDPOINT_H_
#define KSS_CLIENT_ENDPOINT_H_



	void executeKssCommandMount(char * , ListenSocket ls );
	void executeKssCommandFormat(char * , ListenSocket ls);
	void executeKssCommandList(char * , ListenSocket ls);
	void executeKssCommandMd5Sum(char * , ListenSocket ls);
	void executeKssCommandTddDump( ListenSocket ls);
	void executeKssCommandUmount(char * , ListenSocket ls);


#endif /* KSS_CLIENT_COMMANDS_H_ */
