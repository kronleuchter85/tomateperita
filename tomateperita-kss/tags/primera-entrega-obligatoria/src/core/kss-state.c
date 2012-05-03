
#include "kss-state.h"
#include "kss-internal-state.h"
#include "linux-commons-socket.h"


	BOOL kssRunningStatus;
	BOOL availableFss;
	int availableVdas;


	BOOL isKssOperationalStatus(){
		return (hasAvailableFss() && hasAvailableVdas());
	}

	void setAvailableFss(BOOL f){
		availableFss = f;
	}
	BOOL hasAvailableFss(){
		return availableFss;
	}

	void setAvailableVdas(int n){
		availableVdas = n;
	}
	void incrementAvaialbleVdasCount(){
		availableVdas++;
	}
	void decrementAvailableVdasCount(){
		availableVdas--;
	}
	BOOL hasAvailableVdas(){
		return (availableVdas > 0);
	}



	void setKssRunningStatus(int status){
		kssRunningStatus = status;
	}
	int getKssRunningStatus(){
		return kssRunningStatus;
	}
	BOOL isKssRunningStatus(){
		return (getKssRunningStatus() == KSS_STATUS_RUN);
	}

	void finishVdas(){
	}

	void finishFss(){
	}

	void finishKernel(){
		closeServerConnections();

		setKssRunningStatus(FALSE);
		setAvailableFss(FALSE);
		setAvailableVdas(0);
		finishVdas();
		finishFss();
	}
