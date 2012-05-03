/*
 * fss-state.c
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */
#include <unistd.h>
#include "linux-commons.h"
#include "linux-commons-socket.h"

	Bool fssRunningStatus;

	ListenSocket fileSystemSocket;


	Bool isFssRunningStatus(){
		return fssRunningStatus;
	}

	void setFssRunningStatus(Bool s){
		fssRunningStatus = s;
	}


	void killAndWait(int seconds){
		setFssRunningStatus(FALSE);
		sleep(seconds);
	}

	void setFileSystemSocket(ListenSocket l){
		fileSystemSocket = l;
	}

	ListenSocket getFileSystemSocket(){
		return fileSystemSocket;
	}
