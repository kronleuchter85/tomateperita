/**
 * socket-validation.c
 *
 * Validaciones de sockets....
 *
 */

#include "socket-validation.h"


	/**
	 * funciones....
	 */

	int validateAddressInfo(int code){
		if ( code != 0 ) {
			//printf("getaddrinfo fallo con el error: %d\n", code);
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateSocketCreation(ListenSocket ls){
		if (ls == INVALID_SOCKET) {
			//printf("socket failed con el error: %ld\n", WSAGetLastError());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateSocketBinding(int code){
		if (code == SOCKET_ERROR) {
			//printf("el binding fallo con el error: %d\n", WSAGetLastError());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateSocketListening(int code){
		if(code == SOCKET_ERROR) {
			//printf("el listening fallo con el error: %d\n", WSAGetLastError());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateSocketAcceptance(ListenSocket s){
	   if (s == INVALID_SOCKET) {
			//printf("la aceptacion del cliente fallo con el error: %d\n", WSAGetLastError());			
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	 