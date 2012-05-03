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
#include "linux-commons-errors.h"
#include "linux-commons-logging.h"
#include "kss-client-utils.h"




	void callKssOperationMount(char * arg , ListenSocket ls ){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

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





	void callKssOperationUmount(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
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





	void callKssOperationFormat(char * arg , ListenSocket ls){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
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




	void callKssOperationList(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
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





	void callKssOperationTddDump(ListenSocket ls){
		Iterator * ite;
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_TDD_DUMP , NULL);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
					debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		printf("Registros de la TDD (un registro por linea):\n");
		printf("--------------------------------------------------------------------------------------\n");
		printf("File Handle ID: \t File Name: \t VDA Name: \t Opening Mode: \t File Size: \t File Sectors:\n");

		while (equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){

			ite = buildIterator(resp->returnValues);

			while ( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS) &&
					hasMoreElements(ite) ){

				char * record = next(ite);
				printf("%s \t \t" , record);
			}

			printf("\n");
			resp = receiveMpsMessages(ls , req ,validator);
			if( resp->returnValues == NULL ){
				break;
			}
		}

		if (equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_ERROR)){
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}





	void callKssOperationMd5Sum(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

/*
		MpsRequest * req = buildMpsRequest( generateRandomKey(16) , SHELL_MD5_SUM , params);
		MpsResponse * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		Iterator * ite = buildIterator(resp->returnValues);

		//1. Pedirle al kss el archivo
		//2. Ciclo para recibirlo

		//3. Imprimir el hash MD5.
		 */
		Iterator * ite = buildIterator( params );
		char * fileName = next(ite);
		printf("%s\n" , fileName);
		char * md5CommandAndFilename = concatAll( 3 , "md5sum" , " " , fileName);
		system(md5CommandAndFilename);
/*
		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		if( equalsStrings(resp->statusCode , MPS_RESPONSE_STATUS_CODE_SUCCESS)){
			info("Resultado satisfactorio ");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}

		*/

	}
