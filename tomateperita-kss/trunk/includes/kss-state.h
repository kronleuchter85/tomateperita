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



	void setFssServerSocket(ListenSocket l);
	void setVdaServerSocket(ListenSocket l);
	void setShellServerSocket(ListenSocket l);
	void setFtpServerSocket(ListenSocket l);

	ListenSocket getFssServerSocket();
	ListenSocket getVdaServerSocket();
	ListenSocket getShellServerSocket();
	ListenSocket getFtpServerSocket();



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


	Bool hasAvailableVdas();

#endif /* KSS_STATE_H_ */
