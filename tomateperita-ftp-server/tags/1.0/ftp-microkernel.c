/**
 * ftp-microkernel.c
 *
 * Punto de entrada de la aplicacion. Es el 'Main'...
 * Tiene señales de control manipulables mediante las funciones
 * setStatus y getStatus, que modifican la variable KERNEL_STATUS.
 *
 */


#include "ftp.h"



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


	/**
	 * Esta funcion es invocada cuando se presiona CTRL-C
	 * para matar el server manualmente por el administrador
	 * por lo cual los pasos seguidos son la finalizacion 
	 * explicita del kernel y la finalizacion del servicio
	 * con el correspondiente cierre de recursos...
	 */
	void stopService(int s){		
		
		printf("[LOG: Finalizando servicio FTP por peticion del administrador ]\n");
		
		finalizeService();
		setKernelStatus(KERNEL_STATUS_INTERRUPT_BY_EXTERNAL_REQUEST);

		_endthreadex(0);
	}


	int main(void){
		
		/**
		 * Registramos el evento de presionar
		 * CTRL-C con la llamada a la funcion
		 * stopService...
		 */
		signal(SIGINT , &stopService);

		startKernel();	
		startService();
		
		/**
		 * Si ocurre algo que modifica las señales de control
		 * entonces el servicio es detenido cambiando el
		 * estado del kernel y se termina de ejecutar la
		 * funciuon startService, por lo cual el siguiente paso
		 * para la finalizacion del servicio es invocar 
		 * a finalizeService...
		 */		
		if(getKernelStatus() == KERNEL_STATUS_INTERRUPT_BY_INTERNAL_REQUEST){
			
			printf("[LOG: Finalizando servicio FTP por condicion interna al programa]\n");
			finalizeService();
		}

		printf("[LOG: Liberando recursos ocupados por el Server ]\n");

		/**
		 * Liberamos los recursos ocupados por todos los threads
		 * que se estaban ejecutando
		 */
		releaseClientServiceResources();

		Sleep(4000);

		return 0;
	}

