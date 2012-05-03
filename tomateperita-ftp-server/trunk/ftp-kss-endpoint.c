#include "windows-commons.h"
#include "windows-commons-file.h"
#include "windows-commons-mps.h"
#include "windows-commons-list.h"
#include "windows-commons-remote-operations.h"
#include "windows-commons-errors.h"
#include "ftp-state.h"
#include "ftp-commons.h"


	/*
	 * Closing ...
	 */
	void callKssOperationCloseFile(HeapHandler hh, int handleId , RuntimeErrorValidator * validator){
	
		List params = buildStringsListWithValues(hh , 1 , intToString(handleId , getGlobalHeap()));

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , hh) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_CLOSE , params , hh);

		message = sendAndReceiveMpsMessages( getKssConnection() ,message , validator , hh);

		if(hasError(validator))			
			//return -1;
			return;

		if(message == NULL || message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR){
			setError(validator, "Ocurrio un error reciviendo la respuesta del kss en la operacion SysClose");
			return;
		}

		info(concat(hh , 3, "El archivo de fileHandleId:" , intToString(handleId , hh) , " se cerro correctamente.") );
	}

	/*
	 * Opening...
	 */

	int callKssOperationOpenFile(HeapHandler hh, char * path , int mode , RuntimeErrorValidator * validator){
		
		Iterator * ite = NULL;
		int handleId ;

		List params = buildStringsListWithValues(hh, 2 , intToString(mode , hh) , path);

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , hh) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_OPEN , params ,hh);

		message = sendAndReceiveMpsMessages(getKssConnection() , message , 
			validator , hh);


		if( hasError(validator)){			
			return -1;
		}

		if(message == NULL || message->statusCode != MPS_RESPONSE_STATUS_CODE_SUCCESS){
			setError(validator , "Ha ocurrido un error en el envio del mensaje llamando al metodo OpenFile del kss");
			return -1;
		}

		ite = buildIterator(message->commands , hh);
		handleId = atoi( hasMoreElements(ite) ? next(ite) : "-1");

		if(handleId > 0 ){
			return handleId;
		}else{
			return -1;
		}
	}

	
	/*
	 * Writting...
	 *
	 */

	void callKssOperationWrite(HeapHandler hh, int handleId , FileBlock * block , RuntimeErrorValidator * validator){
		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , hh) , 
				MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_WRITE , NULL , hh);
		
		block->fileHandleId = handleId;
		message->block = block;

		/*
		 * Informamos que se trata de este tipo de mensaje
		 * SysCall_Write
		 */
		sendMpsMessage(message , getKssConnection() , validator , hh);

		/*
		 * Enviamos el bloque
		 */
		message = sendBlockAndReceiveMpsMessage( getKssConnection() ,message, validator, hh);
		
		if(!hasError(validator) && (message == NULL || message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR)){
			setError(validator, "Ocurrio un error reciviendo la respuesta del kss en la operacion SysClose");			
		}
	}



	/*
	 * Listing....
	 *
	 */
	List callKssOperationList(HeapHandler hh, char * path , RuntimeErrorValidator * validator){
	
		List params = buildStringsListWithValues(hh , 1 , path);

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , hh ) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_FLIST , params , hh);

		message = sendAndReceiveMpsMessages(getKssConnection() , message , validator , hh);

		if(hasError(validator) || message == NULL || message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR){
			setError(validator , "Se origino un error al recivir la respuesta en la llamada SYS_FLIST");
			return NULL;
		}

		return message->commands;
	}


	/*
	 * Reading....
	 *
	 */

	int callKssOperationStartReadingProcess(HeapHandler hh, char * path  , RuntimeErrorValidator * validator){
		
		int handleId = callKssOperationOpenFile( hh , path , OPENING_MODE_READ , validator);
		
		List params = buildStringsListWithValues(hh , 1 , intToString(handleId , hh));

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , hh) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_READ , params , hh);

		sendMpsMessage(message, getKssConnection() , validator, hh );
		return handleId;
	}


	FileBlock * callKssOperationRead(HeapHandler hh){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator(hh );
		MpsMessage * receiving = receiveMpsBlockMessage(getKssConnection(), validator , hh);

		if(receiving != NULL)
			return receiving->block;
		else
			return NULL;
	}




	/*
	 * Business Write
	 */

	void doBusinessOperationWrite(HeapHandler hh, char * path , FileBlock * block , RuntimeErrorValidator * validator){
		
		int handleId = callKssOperationOpenFile(hh, path , OPENING_MODE_WRITE , validator);

		callKssOperationWrite(hh, handleId , block , validator);

		callKssOperationCloseFile(hh , handleId , validator);

		if(hasError(validator)){
			error(validator->errorDescription);
		}
	}



