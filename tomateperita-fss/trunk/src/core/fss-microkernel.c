/*
 ============================================================================
 Name        : fss-microkernel.c
 Author      : 
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "linux-commons.h"
#include "linux-commons-logging.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"

#include "fss-configuration.h"
#include "fss-state.h"
#include "fss-utils.h"
#include "fss-fs-management.h"
#include "fss-kss-entrypoint.h"


	void setupDirectoryIndexes(){
		if(existsFile(getDataDirectory())){
			if(isInfoEnabled()) info(concatAll( 3, "Usando " , getDataDirectory() , " como directorio de indices"));
		}else{
			if(isInfoEnabled()) info(concatAll(2 , "No existe el directorio " , getDataDirectory()));

			if(createDirectory(getDataDirectory())){
				if(isInfoEnabled()) info(concatAll(2 , "Se crea " , getDataDirectory()));
			}else{
				if(isInfoEnabled()) info(concatAll(2 , "No se pudo crear el directorio " , getDataDirectory()));
				killAndWait(1);
				return;
			}
		}
	}

	void closeKss(){
		info("cerrando");
		close(getFileSystemSocket());
		exit(0);
	}


	void closingEvents(){
		signal(SIGTERM , closeKss);
		signal(SIGKILL , closeKss);
		signal(SIGQUIT , closeKss);
		signal(SIGINT , closeKss);
	}



	void configure(RuntimeErrorValidator * v){
		setupConfiguration(v);
		closingEvents();
		if(hasError(v)){
			return;
		}

		//setLoggingLevelEnabled(LOGGING_LEVEL_INFO);
		setFssRunningStatus(TRUE);
		setupDirectoryIndexes();

		if(isInfoEnabled())
			info("Conectando con modulo KSS");

		ListenSocket s = openClientConnection(getKssHost() , getKssPort());

		if(s == INVALID_SOCKET){
			setError(v , "no se puede abrir la coneccion");
			return;
		}

		MpsHandshake * handshake = buildMpsHandhake(generateRandomKey(16) , FSS_HANDSHAKE);
		handshake = sendAndReceiveMpsHandshake(handshake , s , v);

		if(handshake != NULL && handshake->statusCode == MPS_RESPONSE_STATUS_CODE_SUCCESS ){
			info(concatAll(2 , "Handshake realizado satisfactoriamente con DescriptorId: " , handshake->descriptorId));
			setFileSystemSocket(s);
		}else{
			setError(v , "Hubo un problema y no se pudo realizar el handshake");
			return;
		}
	}



	void executeOperation(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator){

		if(isOperationExistFile(request)){
			executeOperationIsExistingFile(ls , request , validator);
		}else if(isOperationAssignAndGetSectors(request)){
			executeOperationAssignAndGetSectors(ls , request, validator);
		}else if(isOperationCreateFile(request)){
			executeOperationCreateFile(ls , request, validator);
		}else if(isOperationDeleteFile(request)){
			executeOperationDeleteFile(ls , request, validator);
		}else if(isOperationFormat(request)){
			executeOperationFormat(ls , request, validator);
		}else if(isOperationListDirectory(request)){
			executeOperationListDirectory(ls , request, validator);
		}else if(isOperationGetFileInformation(request)){
			executeOperationGetFileInformation(ls , request, validator);
		}else if(isOperationHasVdaValidFormat(request)) {
			executeOperationHasVdaFormat(ls , request , validator);
		}else if(isOperationUpdateFileSize(request)) {
			executeOperationUpdateFileSize(ls , request , validator);
		} else{
			executeInvalidOperation(ls , request, validator);
		}
	}


	void launchTest(){
		do{

			sleep(2);
			//testFormat();
			//testCreate();
			//testCreatingBitVector();
			//testUpdateSize();
			//testGettingFileInformation();
			//testAddingSectors();
			//testListingFiles();
			//testFreeingSectors();
			//testGetCurrentSize();
			//testListImplementation();
			//testListImplementation2();
			//testTokenizingPathFiles();
			//testRandomKey();
			//testLists();
			//testSearchingFiles();
			//testSerializationList();
			testFilesTransferMps2();
		}while(isFssRunningStatus());
	}

	void launch(){
		RuntimeErrorValidator * validator = NULL;

		if(isInfoEnabled())
			info("Quedando a la espera de peticiones para realizar operaciones FSS");

		MpsMessage * request = NULL;

		while(isFssRunningStatus()){
			validator = buildErrorSuccessValidator();
			request = receiveMpsMessage(getFileSystemSocket(), validator);

			if(request == NULL || hasError(validator)){
				error("Se produjo un error en la recepcion, finalizando la aplicacion");
				close(getFileSystemSocket());
				exit(1);
			}

			if(isDebugEnabled())
				debug("Reciviendo peticion MPS");

			if(isDebugEnabled())
				debug(concatAll(3 , "Header.DescriptorId: '" , strndup(request->descriptorId , 16) , "'"));

			if(isDebugEnabled())
				debug( concatAll(3 , "Payload.Arguments: '" , formatListToPreetyString(request->commands) , "'"));

			if(isDebugEnabled())
				debug(concatAll(3 , "Payload.OperationName: '" , request->operationName , "'"));

			executeOperation(getFileSystemSocket() , request , validator);
		}

	}




	int main(void) {

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		configure(validator);

		if(hasError(validator)){
			error(validator->errorDescription);
			return EXIT_FAILURE;
		}

		launch();

		///launchTest();

		return EXIT_SUCCESS;
	}



	Bool isOperationExistFile(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_FILE_CHECK_EXISTENCE);
	}
	Bool isOperationGetFileInformation(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_FILE_INFORMATION);
	}
	Bool isOperationDeleteFile(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_FILE_DELETION);
	}
	Bool isOperationCreateFile(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_FILE_CREATION);
	}
	Bool isOperationListDirectory(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_DIRECTORY_LISTING);
	}
	Bool isOperationFormat(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_FORMAT);
	}
	Bool isOperationAssignAndGetSectors(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_SECTORS_ASSIGN_AND_GET);
	}

	Bool isOperationUpdateFileSize(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_UPDATE_FILE_SIZE);
	}
	Bool isOperationHasVdaValidFormat(MpsMessage * req){
		return equalsStrings(req->operationName , FSS_HAS_VALID_FORMAT);
	}


