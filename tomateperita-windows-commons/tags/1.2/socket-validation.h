/**
 * socket-validation.h
 *
 * Define las funciones de validacion necesarias para el manejo
 * de sockets.
 *
 */


#include "headers-commons.h"
#include "validation.h"


	/**
	 * Validacion de los sockets........
	 */
	int validateAddressInfo(int code);
	int validateSocketCreation(ListenSocket ls);
	int validateSocketBinding(int code);
	int validateSocketListening(int code);
	int validateSocketAcceptance(ListenSocket s);