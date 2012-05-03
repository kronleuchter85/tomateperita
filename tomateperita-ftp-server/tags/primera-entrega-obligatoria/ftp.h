/**
 * ftp-microkernel.h
 *
 * Constantes y funciones que controlan se�ales de control
 * del microkernel del servidor.
 *
 */


	/**
	 * funciones que controlan las se�ales de control....
	 */
	int getKernelStatus();
	void setKernelStatus(int status);
	int isKernelStatusContinue();
	void startKernel();

	int startService();
	void finalizeService();
	void releaseClientServiceResources();


