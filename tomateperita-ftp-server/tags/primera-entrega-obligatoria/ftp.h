/**
 * ftp-microkernel.h
 *
 * Constantes y funciones que controlan señales de control
 * del microkernel del servidor.
 *
 */


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


