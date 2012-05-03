/*
 * test-mps-files-transfer.c
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

#include "fss-configuration.h"
#include "fss-state.h"
#include "fss-utils.h"
#include "fss-fs-management.h"
#include "fss-kss-entrypoint.h"



	char * imagePath = "/home/gonzalo/images/anonimo.jpg";


	int number = 1;

	void testFilesTransferMps2(){

		MpsMessage * req = buildMpsMessage(generateRandomKey(16) ,
				MPS_RESPONSE_STATUS_CODE_SUCCESS, "snd_img1" ,
				buildStringsListWithValues(1, imagePath));

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		MpsMessage * response = sendMpsMessageAndReceiveBlock(getFileSystemSocket() , req, validator);

		char * imgName = getDataDirectory();
		imgName = concatAll(4, imgName , "/IMAGEN_TRANSFERIDA-", itoa(number) , ".jpg");
		File * file1 = fopen(imgName , "w");

		info(concatAll(3 , "DescriptorId = '" , strndup(response->descriptorId , 16) , "'"));
		info(concatAll(2 , "Creando imagen: " , imgName));

		while(response->block != NULL){

			FileBlock * myBlock = response->block;

			int written1 = fwrite(myBlock->sector1.data, sizeof(char) , myBlock->sector1.dataLength , file1);
			int written2 = fwrite(myBlock->sector2.data, sizeof(char) , myBlock->sector2.dataLength , file1);

			info(concatAll(5, "Bytes escritos: { sector1 = " , itoa(written1) , " , sector2 = " , itoa(written2) , " }"));

			response = receiveMpsBlockMessage(getFileSystemSocket() , validator);
		}

		char bufferLoco[15];
		bzero(bufferLoco , 15);
		int written3 = fwrite(bufferLoco , sizeof(char) , 15 , file1);
		printf("Se han escrito %i bytes de buffer Loco\n" , written3);

		info("Cerrando archivo");

		fclose(file1);
		number++;
	}
