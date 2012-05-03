
#include "linux-commons-socket.h"

#include "kss-state.h"
#include "kss-utils.h"

#include "kss-vda-state.h"

	ListenSocket fssServerSocket;
	ListenSocket ftpsServerSocket;
	ListenSocket vdasServerSOcket;
	ListenSocket shellServerSOcket;

	void setFssServerSocket(ListenSocket l){
		fssServerSocket = l;
	}
	void setVdaServerSocket(ListenSocket l){
		vdasServerSOcket = l;
	}
	void setShellServerSocket(ListenSocket l){
		shellServerSOcket = l;
	}
	void setFtpServerSocket(ListenSocket l){
		ftpsServerSocket = l;
	}
	ListenSocket getFssServerSocket(){
		return fssServerSocket;
	}
	ListenSocket getVdaServerSocket(){
		return vdasServerSOcket;
	}
	ListenSocket getShellServerSocket(){
		return shellServerSOcket;
	}
	ListenSocket getFtpServerSocket(){
		return ftpsServerSocket;
	}




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



