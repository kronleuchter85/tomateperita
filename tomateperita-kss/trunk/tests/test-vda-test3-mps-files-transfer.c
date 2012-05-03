/*
 * test3-mps-files-transfer.c
 *
 *  Created on: 27/07/2011
 *      Author: gonzalo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-logging.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"
#include "linux-commons-remote-services.h"

#include "kss-utils.h"




	void testFilesTransferMps2(){


		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsMessage * request = receiveMpsMessage(getFileSystemConnection() , validator);

		Iterator * ite = buildIterator(request->commands);
		char * path = hasMoreElements(ite) ? next(ite) : NULL;

		if(path == NULL){
			setError(validator , "El puto archivo vino en null");
			return ;
		}

		info(concatAll(3 , "DescriptorId = '" , strndup(request->descriptorId , 16) , "'"));
		info(concatAll(2, "Archivo solicitado: " , path));


		File * file1 = fopen(path , "r");

		char buffer1[512];
		char buffer2[512];

		while(! feof(file1) ){

			bzero(buffer1 , 512);
			bzero(buffer2 , 512);

			int readedS1 = fread(buffer1, sizeof(char) , 512 , file1);
			int readedS2 = fread(buffer2, sizeof(char) , 512 , file1);

			info(concatAll(5 , "Enviando contenido: { Sector1 = " , itoa(readedS1) , " , Sector2 = " , itoa(readedS2) , "}"));

			FileBlock * block = buildEmptyFileBlock();
			setFileBlockContents(block , buffer1 , buffer2 , readedS1 , readedS2);

			MpsMessage * response = buildMpsMessage(request->descriptorId,
					MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);
			response->block = block;

			sendMpsBlockMessage(getFileSystemConnection() , response , validator);

		}

		MpsMessage * response = buildMpsMessage(request->descriptorId,
			MPS_RESPONSE_STATUS_CODE_SUCCESS , request->operationName , NULL);

		sendMpsMessage(response , getFileSystemConnection() , validator);

		fclose(file1);
	}
