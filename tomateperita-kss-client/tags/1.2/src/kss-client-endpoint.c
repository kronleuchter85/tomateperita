/*
 * kss-client-commands.c
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <string.h>

#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons.h"
#include "linux-commons-remote-services.h"
#include "linux-errors.h"
#include "linux-commons-logging.h"

#include "kss-client-utils.h"


	void executeKssCommandMount(char * arg , ListenSocket ls ){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);
				//getFirstParameter("Dispositivo a Montar");

		MpsRequest * req = buildMpsRequest(generateRandomKey(16) , SHELL_MOUNT , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req , validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->returnValues);

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			printf("Dispositivo montado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}

	void executeKssCommandUmount(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		//List params = getFirstParameter("Dispositivo o directorio a Desmontar");
		List params = buildStringsListWithValues(1 , arg);

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_UMOUNT , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->returnValues);

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			printf("Dispositivo desmontado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}

	void executeKssCommandFormat(char * arg , ListenSocket ls){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		//List params = getFirstParameter("Dispositivo a Formatear");
		List params = buildStringsListWithValues(1 , arg);

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_FORMAT , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->returnValues);

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			printf("Dispositivo formateado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}

	void executeKssCommandList(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		//List params = getFirstParameter("Dispositivo o directorio a Listar");
		List params = buildStringsListWithValues(1 , arg);

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_LS , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->returnValues);

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){

			char * serializedFiles = next(ite);
			List files = deserializeList(serializedFiles);
			ite = buildIterator(files);

			printf("Contenido del directorio:\n");
			printf("--------------------------------------------------------------------------------------\n");

			while(hasMoreElements(ite)){
				char * elem = next(ite);
				printf("%s\n " , trim(elem));
			}
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}


	void executeKssCommandTddDump(ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_TDD_DUMP , NULL);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		Iterator * ite = buildIterator(resp->returnValues);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			info("Resultado satisfactorio ");
		}else{
			printf("%s\n" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}

	void executeKssCommandMd5Sum(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		//List params = getFirstParameter("Nombre del archivo");
		List params = buildStringsListWithValues(1 , arg);

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_MD5_SUM , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		Iterator * ite = buildIterator(resp->returnValues);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			info("Resultado satisfactorio ");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}

	}
