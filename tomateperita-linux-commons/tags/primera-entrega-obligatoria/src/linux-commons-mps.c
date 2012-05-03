/*
 * linux-syscall-protocol.c
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-mps.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


	/**
	 * Construye un mensaje mps
	 */
	MpsMessage buildMessage(long idDescriptor, int payloadDescriptor,
			char * payload){
		MpsMessage message;

		sprintf(&message.payloadDescriptor , "%i" , payloadDescriptor);
		sprintf(message.payload , "%s" , payload);
		message.payloadLenght = strlen(payload);

		return message;
	}


	/**
	 * ejecuta la llamada remotea sys_open.
	 * Devuelve un entero que determina el fd en caso de ser positivo.
	 * Que hubo un error en caso de ser negativo.
	 */
	int remoteCallSysOpen(ListenSocket l, char mode , char * path){

		//<modo> + <un espacio> + <ruta>
		char * content =(char *)malloc(sizeof(char) + strlen(path) + 1);
		sprintf(content , "%c %s" , mode , path);
		MpsMessage message = buildMessage(1,SYS_OPEN , content);
		send(l , &message , sizeof(message) , 0);
		recv(l , &message , sizeof(MpsMessage) ,0 );

		//retornamos la respuesta entera del campo payload
		return atoi(message.payload);
	}

	/*
	 * Ejecuta la llamada remote sys_read con el fd pasado como
	 * parametro.
	 * Devuelve el contenido del archivo.
	 */
	char * remoteCallSysRead(ListenSocket l, int fd ){

		char * content = malloc(sizeof(char));
		sprintf(content , "%i" , fd);
		MpsMessage m = buildMessage(1 , SYS_READ , content);
		send(l , &m , sizeof(MpsMessage) , 0);
		recv(l , &m , sizeof(MpsMessage) ,0 );

		return m.payload;
	}

	/*	int remoteCallSysWrite(ListenSocket l, int fd , char * buffer ){

	}
	void remoteCallSysClose(ListenSocket l, int fd , int * status);
	void remoteCallSysFlush(ListenSocket l, int fd , int * status);*/
