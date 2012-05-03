/**
 * ftp-state.c
 *
 * Funciones para el Heap global
 * y funciones para el KSS.
 *
 */

#include "ftp-state.h"
	
	/**
	 * Heap de la aplicacion global
	 */

	HeapHandler heap;

	HeapHandler getGlobalHeap(){
		return heap;
	}

	void setGlobalHeap(HeapHandler h){
		heap = h;
	}

	/**
	 * Socekts para KSS
	 */
	
	ListenSocket kssConnection;
	
	void setKssConnection(ListenSocket sock){
		kssConnection = sock;
	}

	ListenSocket getKssConnection(){
		return kssConnection;
	}

	/**
	 * Señal de control
	*/

	int KERNEL_STATUS;

	int getKernelStatus(){
		return KERNEL_STATUS;
	}
	void setKernelStatus(int status){
		KERNEL_STATUS = status;
	}
	int isKernelStatusContinue(){
		return (getKernelStatus() == KERNEL_STATUS_CONTINUE);
	}

	void startKernel(){
		setKernelStatus( KERNEL_STATUS_CONTINUE );
	}