/*
 * fss-state.h
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-socket.h"
#ifndef FSS_STATE_H_
#define FSS_STATE_H_

	Bool isFssRunningStatus();
	void setFssRunningStatus(Bool s);
	void killAndWait(int seconds);

	void setFileSystemSocket(ListenSocket l);
	ListenSocket getFileSystemSocket();


#endif /* FSS_STATE_H_ */
