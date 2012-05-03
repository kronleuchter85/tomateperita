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

		MpsMessage * req = buildMpsMessage(generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_MOUNT , params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req , validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->commands);

		if( resp->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			printf("Dispositivo montado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}




	void callKssOperationUmount(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

		MpsMessage * req = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_UMOUNT , params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->commands);

		if( resp->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			printf("Dispositivo desmontado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}




	void callKssOperationFormat(char * arg , ListenSocket ls){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

		MpsMessage * req = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_FORMAT , params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->commands);

		if( resp->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			printf("Dispositivo formateado\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}




	void callKssOperationList(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

		MpsMessage * req = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_LS , params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->commands);

		if( resp->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){

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
		Iterator * ite = NULL;
		int flag = 0;
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsMessage * msg = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_TDD_DUMP , NULL);

		msg = sendAndReceiveMpsMessages(ls , msg ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , msg->statusCode));

		printf("Registros de la TDD (un registro por linea):\n");
		printf("--------------------------------------------------------------------------------------\n");
		printf("File Handle ID: \t File Name: \t VDA Name: \t Opening Mode: \t File Size: \t File Sectors:\n");

		while ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			ite = buildIterator(msg->commands);
			while ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS
					&& hasMoreElements(ite) ){

				char * record = next(ite);
				printf("%s \t \t" , record);
				flag = 1;
			}

			printf("\n");

			if (flag != 0){
				msg = receiveMpsMessage(ls  ,validator);
			} else {
				printf("(Tabla de Descriptores vacia.)");
				break;
			}

			if( msg->commands == NULL ){
				break;
			}
		}

		if ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR ){
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}



	void callKssOperationMd5Sum( char * arg , ListenSocket ls ){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(2 , "0" , arg);
		FILE * newFile = fopen( arg , "w" );

		MpsMessage * msg = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_OPEN , params);

		msg = sendAndReceiveMpsMessages(ls , msg , validator);

		if ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR ){
			setError(validator , "Error en la solicitud OpenFile al KSS.");
			error(validator->errorDescription);
			return;
		}

		Iterator * ite = buildIterator(msg->commands);
		int handleId = atoi( hasMoreElements(ite) ? next(ite) : "-1");

		if ( handleId < 0 ){
			setError(validator , "Error en la solicitud OpenFile al KSS.");
			error(validator->errorDescription);
			return;
		}

		params = buildStringsListWithValues( 1 , itoa(handleId));

		msg = buildMpsMessage( generateRandomKey(16) , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
				KSS_SYS_CALL_READ , params);

		msg = sendMpsMessageAndReceiveBlock(ls , msg , validator);

		if ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR ){
			setError(validator , "Error en la solicitud ReadFile al KSS.");
			error(validator->errorDescription);
			return;
		}

		FileBlock * block = msg->block;

		while( block != NULL && !hasError(validator) ){
			if( block->sector1.dataLength > 0 ){
				fwrite(msg->block->sector1.data , msg->block->sector1.dataLength , 1 , newFile );

				if( block->sector2.dataLength > 0 ){
					fwrite(msg->block->sector2.data , msg->block->sector2.dataLength , 1 , newFile );
				}
			}

			msg = receiveMpsBlockMessage(ls ,validator);
			block = msg->block;
		}

		if ( hasError(validator) ){
			setError(validator , "Fallo en la recepcion del archivo para calcular el hash.");
			error(validator->errorDescription);
			return;
		} else {
			char * fileName = arg;
			char * md5CommandAndFilename = concatAll( 3 , "md5sum" , " " , fileName);
			system(md5CommandAndFilename);
		}

		params = buildStringsListWithValues( 1 , itoa(handleId));

		msg = buildMpsMessage(generateRandomKey(16) , MPS_RESPONSE_STATUS_CODE_SUCCESS ,
				KSS_SYS_CALL_CLOSE , params);

		msg = sendAndReceiveMpsMessages(ls , msg , validator);

		if( msg->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR ){
			setError( validator , "Error en la solicitud CloseFile al KSS.");
			return;
		}
	}



	void callKssOperationDisconnect(char * arg , ListenSocket ls ){

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

		MpsMessage * req = buildMpsMessage(generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_DISCONNECT , params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req , validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));

		Iterator * ite = buildIterator(resp->commands);

		if( resp->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			printf("Dispositivo desconectado.\n");
		}else{
			printf("%s" , ( hasMoreElements(ite) ? (char *) next(ite) : "Ha ocurrido un error"));
		}
	}



	void callKssOperationQuit(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);

		MpsMessage * req = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_QUIT ,params);
		MpsMessage * resp = sendAndReceiveMpsMessages(ls , req ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , resp->statusCode));
	}

	void callKssOperationShowDevices(ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsMessage * msg = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_SHOW_DEVICES , NULL);

		msg = sendAndReceiveMpsMessages(ls , msg ,validator);

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , msg->statusCode));

		Iterator * ite = buildIterator(msg->commands);

		printf("Dispositivos conectados:\n");
		printf("--------------------------------------------------------------------------------------\n");

		if( hasMoreElements(ite)){
			while( hasMoreElements(ite) ){
				char * device = next(ite);
				printf("%s\n" , device);
			}
		} else {
			printf("No hay actualmente dispositivos conectados y sin montar.\n");
			return;
		}
	}

	/*** VIEJA MD5SUM

	void callKssOperationMd5Sum(char * arg , ListenSocket ls){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		List params = buildStringsListWithValues(1 , arg);
		FILE * newFile = fopen( arg , "w" );

		MpsMessage * msg = buildMpsMessage( generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS , SHELL_MD5_SUM , params);

		msg = sendMpsMessageAndReceiveBlock(ls , msg ,validator);

		while ( msg->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS  || !hasError(validator) ){
			fwrite( msg->block->sector1.data , msg->block->sector1.dataLength , 1 , newFile );
			fwrite( msg->block->sector2.data , msg->block->sector2.dataLength , 1 , newFile );

			msg = sendMpsMessageAndReceiveBlock( ls , msg ,validator );
		}

		if ( hasError(validator) ){
			error("Fallo en la recepcion del archivo para calcular el hash.");
		} else {
			char * fileName = arg;
			char * md5CommandAndFilename = concatAll( 3 , "md5sum" , " " , fileName);
			system(md5CommandAndFilename);
		}

		if(isDebugEnabled())
			debug(concatAll(2, "Codigo de resultado: " , msg->statusCode));

		if( msg->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			info("Resultado satisfactorio ");
		}else{
			replyValidationError( ls , msg , validator );
		}
	}
	************/
