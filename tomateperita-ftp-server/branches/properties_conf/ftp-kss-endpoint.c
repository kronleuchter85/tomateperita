#include "windows-commons.h"
#include "windows-commons-file.h"
#include "windows-commons-mps.h"
#include "windows-commons-list.h"
#include "windows-commons-remote-operations.h"
#include "windows-commons-errors.h"



	void callKssOperationCloseFile(int handleId , RuntimeErrorValidator * validator){
	
		List params = buildStringsListWithValues(getGlobalHeap() , 
			1 , intToString(handleId , getGlobalHeap()));

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , getGlobalHeap()) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_CLOSE , 
			params , getGlobalHeap());

		message = sendAndReceiveMpsMessages(message , getKssConnection() , 
			validator , getGlobalHeap());

		if(message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR){
			setError(validator, "Ocurrio un error reciviendo la respuesta del kss en la operacion SysClose");
			return;
		}

		info(concat(3, "El archivo de fileHandleId:" , intToString(handleId , getGlobalHeap())) , " se cerro correctamente.");
	}




	int callKssOperationOpenFile(char * path , int mode , RuntimeErrorValidator * validator){
		
		Iterator * ite = NULL;
		int handleId ;

		List params = buildStringsListWithValues(getGlobalHeap() , 
			2 , intToString(mode , getGlobalHeap()) , path);

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , getGlobalHeap()) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_OPEN , 
			params , getGlobalHeap());

		message = sendAndReceiveMpsMessages(message , getKssConnection() , 
			validator , getGlobalHeap());

		if(message->statusCode != MPS_RESPONSE_STATUS_CODE_SUCCESS){
			setError(validator , "Ha ocurrido un error en el envio del mensaje llamando al metodo OpenFile del kss");
			return -1;
		}

		ite = buildIterator(message->commands , getGlobalHeap());
		handleId = atoi( hasMoreElements(ite) ? next(ite) : "-1");

		if(handleId > 0 ){
			return handleId;
		}else{
			return -1;
		}
	}







	List callKssOperationList(char * path , RuntimeErrorValidator * validator){
	
		List params = buildStringsListWithValues(getGlobalHeap() , 
			1 , path);

		MpsMessage * message = buildMpsMessage(generateRandomKey(16 , getGlobalHeap()) , 
			MPS_RESPONSE_STATUS_CODE_SUCCESS , KSS_SYS_CALL_FLIST , 
			params , getGlobalHeap());

		message = sendAndReceiveMpsMessages(getKssConnection() , message , 
			validator , getGlobalHeap());

		if(message->statusCode == MPS_RESPONSE_STATUS_CODE_ERROR){
			setError(validator , "Se origino un error al recivir la respuesta en la llamada SYS_FLIST");
			return NULL;
		}

		return message->commands;
	}






	void callKssOperationWrite(int handleId, FileBlock * block){
	}


	FileBlock * callKssOperationRead(int handleId){
		return NULL;
	}





