
#include "linux-commons-socket.h"

#include "kss-state.h"
#include "kss-utils.h"

#include "kss-vda-state.h"


	/*
	 * Status..
	 */
	Bool kssRunningStatus;

	/*
	 * File system
	 */
	ListenSocket fileSystemConnection;



	Bool isKssOperationalStatus(){
		return (hasAvailableFss() && hasAvailableVdas());
	}
	void setKssRunningStatus(Bool status){
		kssRunningStatus = status;
	}
	Bool isKssRunningStatus(){
		return (kssRunningStatus == TRUE);
	}


	ListenSocket getFileSystemConnection(){
		return fileSystemConnection;
	}
	void setFileSystemConnection(ListenSocket c){
		if(c > 0){
			fileSystemConnection = c;
		}
	}
	Bool hasAvailableFss(){
		return (getFileSystemConnection() > 0);
	}



