/*
 * kss-state.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"
#include "kss-vda-service.h"

#ifndef KSS_STATE_H_
#define KSS_STATE_H_


	/**
	 * Operational State
	 */
	Bool isKssOperationalStatus();
	void setKssRunningStatus(int status);
	Bool isKssRunningStatus();

	/**
	 * available Fss
	 */
	Bool hasAvailableFss();
	void setFileSystemConnection(ListenSocket c);
	ListenSocket getFileSystemConnection();


#endif /* KSS_STATE_H_ */
