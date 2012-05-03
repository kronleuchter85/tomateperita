/**
 * ftp-microkernel.h
 *
 * Constantes y funciones que controlan señales de control
 * del microkernel del servidor.
 *
 */

#include "headers-commons.h"
#include "ftp-constants.h"
#include "ftp-service-validation.h"
#include "ftp-thread-worker.h"
#include "socket-commons.h"
#include "socket-validation.h"
#include "memory-commons.h"
#include "ftp-client-thread-resources.h"

	/**
	 * funciones que controlan las señales de control....
	 */
	int getKernelStatus();
	void setKernelStatus(int status);
	int isKernelStatusContinue();
	void startKernel();

	int startService();
	void finalizeService();
	void releaseClientServiceResources();


