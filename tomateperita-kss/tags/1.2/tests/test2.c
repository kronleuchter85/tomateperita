/*
 * test2.c
 *
 *  Created on: 12/07/2011
 *      Author: gonzalo
 */

#include "linux-errors.h"
#include "linux-commons.h"

#include "kss-utils.h"
#include "kss-fss-endpoint.h"


	void testFormat(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		callFssOperationFormat("VDA1", 40 , v);

		if(!hasError(v)){
			info("Se ha finalizado el formateo satisfactoriamente");
		}else{
			error(v->errorDescription);
		}

	}


	void testAssignAndGetSectors(){
		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		List freeSectors = callFssOperationAssignAndGetSectors("VDA1" , "archivo1 con espacios.txt", v);

		if(!hasError(v)){
			info(concatAll(3,"Se han traido los sectores correctamente: (" , serializeList(freeSectors , " , ") , ")"));
		}else{
			error(v->errorDescription);
		}
	}


	void testListDirectory(){

		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		List files = callFssOperationGetDirectoryInformation("VDA1", v);

		if(!hasError(v)){

			Iterator * ite = buildIterator(files);

			info("Se han recuperado con exito los archivos del directorio");
			while(hasMoreElements(ite)){
				FileInformation * infof = next(ite);
				info(concatAll(4, "FileName: " , infof->name , " , FileSize: " , ltoa(infof->size)));
			}

		}else{
			error(v->errorDescription);
		}


	}


	void testListInexistingDirectory(){

		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		List files = callFssOperationGetDirectoryInformation("VDA2", v);

		if(!hasError(v)){

			Iterator * ite = buildIterator(files);

			info("Se han recuperado con exito los archivos del directorio");
			while(hasMoreElements(ite)){
				FileInformation * infof = next(ite);
				info(concatAll(2, "FileName: " , infof->name , " , FileSize: " , ltoa(infof->size)));
			}

		}else{
			error(v->errorDescription);
		}


	}




