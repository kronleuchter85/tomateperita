/*
 * kss-state.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"

#ifndef KSS_STATE_H_
#define KSS_STATE_H_

	#define KSS_STATUS_RUN 1
	#define KSS_STATUS_STOP 0

	/**
	 * Operational State
	 */
	BOOL isKssOperationalStatus();

	/**
	 * available Fss
	 */
	void setAvailableFss(BOOL f);
	BOOL hasAvailableFss();

	/**
	 * available Vdas
	 */
	void setAvailableVdas(int n);
	void incrementAvaialbleVdasCount();
	void decrementAvailableVdasCount();
	BOOL hasAvailableVdas();


	/**
	 * Kernel Running state
	 */
	void setKssRunningStatus(int status);
	int getKssRunningStatus();
	BOOL isKssRunningStatus();
	void finishKernel();

#endif /* KSS_STATE_H_ */
