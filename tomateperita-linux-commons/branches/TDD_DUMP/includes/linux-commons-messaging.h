/*
 * linux-commons-mps-2.h
 *
 *  Created on: 21/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-socket.h"


#ifndef LINUX_COMMONS_MPS_2_H_
#define LINUX_COMMONS_MPS_2_H_

	/*
	 * Mantiene un mensaje MPS total con la serializacion
	 * de su header y su payload.
	 */
	typedef struct {
		int length;
		char * headerAndPayload;
	} MpsPackage;

	/*
	 * Representa los campos del header de
	 * un mensaje MPS.
	 */
	typedef struct {
		char descriptorId[16];
		char payloadDescriptor;
		int payloadSize;
	}MpsHeader;

	/*
	 * Representa el payload de un mensaje MPS
	 */
	typedef struct {
		int length;
		char * payloadData;
	}MpsPayload;

	/*
	 * Representa un mensaje respuesta de alto nivel manejado por
	 * la aplicacion.
	 * Una abstraccion total a la capa transporte MPS.
	 */
	typedef struct{
		char statusCode;
		List returnValues;
	}MessageResponse;

	/*
	 * Representa un mensaje de peticion de alto nivel manejado por
	 * la aplicacion.
	 * Una abstraccion total a la capa transporte MPS.
	 */
	typedef struct {
		char * operationName;
		List paramsValues;
	}MessageRequest;

	/*
	 * Representa una abstraccion de alto nivel para un handshake.
	 * Solamente interesa conocer el codigo de proceso que realiza el
	 * handshake y el codigo de estado de retorno.
	 */
	typedef struct{
		char handshakeCode;
	}Handshake;


	/*
	 * builders conversores para la conversion entre mensajes de
	 * alto nivel a bajo nivel
	 */

	MessageRequest * buildRequestByPackage(MpsPackage * stream);
	MpsPackage * buildPackageByRequest(MessageRequest * request);
	MessageResponse * buildResponseByPackage(MpsPackage * stream);
	MpsPackage * buildPackageByResponse(MessageResponse * response);
	MpsPackage * buildPackageByHandshake(Handshake * hand);
	Handshake * buildHandshakeByPackage(MpsPackage * pack);

	/*
	 * senders y receivers de alto nivel
	 */
	void sendResponse(MessageResponse response , ListenSocket ls);
	void sendRequest(MessageRequest request, ListenSocket ls);
	MessageRequest receiveRequest(ListenSocket ls);
	MessageResponse receiveResponse(ListenSocket ls);
	void sendHandshake(Handshake handshake, ListenSocket ls);
	Handshake receiveHandshake(ListenSocket ls);

	/*
	 * sender y receiver de bajo nivel
	 */
	void sendPackage(MpsPackage package, ListenSocket ls);
	MpsPackage receivePackage(ListenSocket ls);


	/*
	 * builders de proposito general de alto nivel
	 */
	Handshake buildHandshake(char code);
	MessageResponse buildResponse(char statusCode , List returnValues);
	MessageRequest buildRequest(char * operationName , List paramValues);






#endif /* LINUX_COMMONS_MPS_2_H_ */
