/**
 * ftp-microkernel.c
 *
 * Punto de entrada de la aplicacion. Es el 'Main'...
 * Tiene señales de control manipulables mediante las funciones
 * setStatus y getStatus, que modifican la variable KERNEL_STATUS.
 *
*/


#include "commons.h"
#include "headers-commons.h"
#include "ftp-constants.h"
#include "ftp-service-validation.h"
#include "ftp-thread-worker.h"
#include "socket-commons.h"
#include "socket-validation.h"
#include "memory-commons.h"
#include "ftp-client-thread-resources.h"
#include "logging.h"
#include "ftp-protocol-constants.h"
#include "ftp-protocol-service.h"
#include "ftp-configuration.h"
#include "ftp-state.h"

	//HeapHandler heap;
	//ListenSocket kssConnection;
	//int KERNEL_STATUS;

	int startService();
	void finalizeService();
	void releaseClientServiceResources();

	/**
	 * Esta funcion es invocada cuando se presiona CTRL-C
	 * para matar el server manualmente por el administrador
	 * por lo cual los pasos seguidos son la finalizacion 
	 * explicita del kernel y la finalizacion del servicio
	 * con el correspondiente cierre de recursos...
	 */
	void stopService(int s){		
		
		if(isInfoEnabled()){
			logInfo("ftp-serivce" , 
				"Finalizando servicio FTP por peticion del administrador" , getGlobalHeap());
		}

		finalizeService();
		setKernelStatus(KERNEL_STATUS_INTERRUPT_BY_EXTERNAL_REQUEST);

		_endthreadex(0);
	}


	void launch(){
	
		setGlobalHeap( createHeap(1024) );		
		setUpConfiguration();

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
			
			if(isInfoEnabled()){
				logInfo("ftp-service" , 
					"Finalizando servicio FTP por condicion interna al programa" , getGlobalHeap());
			}
			
			finalizeService();
		}

		if(isInfoEnabled()){
			logInfo("ftp-service" , "Liberando recursos ocupados por el Server" , getGlobalHeap());
		}

		/**
		 * Liberamos los recursos ocupados por todos los threads
		 * que se estaban ejecutando
		 */
		releaseClientServiceResources();
		Sleep(4000);
		destroyHeap(getGlobalHeap());
	}




	int main(void){		
		
		launch();	
		//setGlobalHeap( createHeap(1024) );	
		//destroyHeap(getGlobalHeap());
		return 0;
	}




