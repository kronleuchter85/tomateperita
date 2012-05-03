/*
 * test1.c
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-errors.h"
#include "linux-commons.h"

#include "kss-utils.h"
#include "kss-fss-endpoint.h"

	void testIsExistingFile(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		Bool result = callFssOperationIsExistingFile("VDA1", "archivo2.txt", v);

		if(result){
			info("El archivo /VDA1/archivo2.txt existe");
		}else{
			if(hasError(v))
				error(v->errorDescription);
			else
				info("El archivo /VDA1/archivo2.txt no existe");
		}
	}


	void testGetFileInformation(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		FileInformation * infoFile = callFssOperationGetFileInformation("VDA1", "archivo2.txt", v);

		if(infoFile != NULL){

			info("Information.Vda: VDA1");
			info("Information.FileName: archivo2.txt");
			info(concatAll(2, "Information.Size: " , ltoa(infoFile->size)));
			info(concatAll(2, "Information.Sectors: " , formatListToPreetyString(infoFile->sectors)));

		}else{
			if(hasError(v))
				error(v->errorDescription);
		}
	}


	void testCreateFile1(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationCreateFile("VDA1", "archivo1 con espacios.txt", v);

		if(hasError(v)){
			error(v->errorDescription);
		}else{
			info("La creacion del archivo concluyo con exito");
		}
	}
	void testCreateFile2(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationCreateFile("VDA1", "archivo2 con espacios.txt", v);

		if(hasError(v)){
			error(v->errorDescription);
		}else{
			info("La creacion del archivo concluyo con exito");
		}
	}
	void testCreateFile3(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationCreateFile("VDA1", "archivo3.txt", v);

		if(hasError(v)){
			error(v->errorDescription);
		}else{
			info("La creacion del archivo concluyo con exito");
		}
	}
	void testCreateFile4(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationCreateFile("VDA1", "archivo4 con espacios.txt", v);

		if(hasError(v)){
			error(v->errorDescription);
		}else{
			info("La creacion del archivo concluyo con exito");
		}
	}

	void testGetFileInformationCreated(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		FileInformation * infoFile = callFssOperationGetFileInformation("VDA1", "archivo1 con espacios.txt", v);

		if(infoFile != NULL){

			info("Information.Vda: VDA1");
			info("Information.FileName: archivo1 con espacios.txt");
			info(concatAll(2, "Information.Size: " , ltoa(infoFile->size)));
			info(concatAll(2, "Information.Sectors: " , formatListToPreetyString(infoFile->sectors)));

		}else{
			if(hasError(v))
				error(v->errorDescription);
		}
	}



	void testDeleteFile(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationDeleteFile("VDA1", "archivo3.txt", v);

		if(hasError(v)){
			error(v->errorDescription);
		}else{
			info("La eliminacion del archivo concluyo con exito");
		}

	}

	void testGetFileInformationDeleted(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		FileInformation * infoFile = callFssOperationGetFileInformation("VDA1", "archivo3.txt", v);

		if(infoFile != NULL){

			info("Information.Vda: VDA1");
			info("Information.FileName: archivo3.txt");
			info(concatAll(2, "Information.Size: " , ltoa(infoFile->size)));
			info(concatAll(2, "Information.Sectors: " , formatListToPreetyString(infoFile->sectors)));

		}else{
			if(hasError(v))
				error(v->errorDescription);
		}
	}
