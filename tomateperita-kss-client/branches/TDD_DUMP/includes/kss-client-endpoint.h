/*
 * kss-client-commands.h
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"

#ifndef KSS_CLIENT_ENDPOINT_H_
#define KSS_CLIENT_ENDPOINT_H_


	void callKssOperationMount(char * , ListenSocket ls );
	void callKssOperationFormat(char * , ListenSocket ls);
	void callKssOperationList(char * , ListenSocket ls);
	void callKssOperationMd5Sum(char * , ListenSocket ls);
	void callKssOperationTddDump( ListenSocket ls);
	void callKssOperationUmount(char * , ListenSocket ls);


#endif /* KSS_CLIENT_COMMANDS_H_ */
