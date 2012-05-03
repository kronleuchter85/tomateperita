/**
 * ftp-service-validation.h
 *
 * Define las funciones especiales
 * para el servicio ftp-service referentes 
 * al manejo de los threads para las peticiones.
 *
 */


#include "headers-commons.h"
#include "socket-commons.h"
#include "commons.h"
#include "ftp-client-thread-resources.h"

	/**
	 * funciones....
	 */
	unsigned __stdcall serviceClientThread( void* pArguments );
