/**
 * ftp-service-validation.c
 *
 * Validaciones del modulo ftp...
 */


#include "ftp-service-validation.h"


	/**
	 * Funciones de validacion....
	 */
	int validateStartup(int code){
		if (code != 0) {
			printf("WSAStartup fallo con errores: %d\n", code);
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}
