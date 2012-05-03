/*
 * kss-internal-state.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */


#include "kss-vda-component.h"

#ifndef KSS_INTERNAL_STATE_H_
#define KSS_INTERNAL_STATE_H_

	void setFssComponentConnection(ListenSocket c);
	void initializeVdas();
	void addVdaComponent(VdaComponent * );


	void setVdaServerConnection(ServerSocket * s);
	void setFtpsServerConnection(ServerSocket * s);
	void setShellServerConnection(ServerSocket * s);
	void closeAndFreeServerConnection(ServerSocket * s);
	void closeVdaServerConnection();
	void closeFtpsServerConnection();
	void closeShellServerConnection();
	void closeServerConnections();


#endif /* KSS_INTERNAL_STATE_H_ */
