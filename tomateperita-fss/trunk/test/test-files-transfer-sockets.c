/*
 * test-files-transfer-sockets.c
 *
 *  Created on: 31/07/2011
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

	void testFilesTransfer(){

		File * file1 = fopen("/home/gonzalo/tomateperita-data/IMAGEN_TRANSFERIDA.jpg" , "w");

		MpsHeader header;

		int effectiveReceived = recv(getFileSystemSocket() , &header , sizeof(MpsHeader) , 0);

		char toWrite[1024];

		while(effectiveReceived == 21 && header.payloadSize > 0){

			if(header.payloadDescriptor == 'b'){

				char operationName[8];
				receiveBytes(getFileSystemSocket() , operationName , 8);

				FileBlock block;
				recv(getFileSystemSocket() , &block , sizeof(FileBlock) , 0);

				memcpy(toWrite , block.sector1.data , 512);
				memcpy(toWrite + 512 , block.sector2.data , 512);
				fwrite(toWrite, sizeof(char) , 1024 , file1);


				header.payloadDescriptor = '0';
				send(getFileSystemSocket() , &header , sizeof(MpsHeader) , 0);

				effectiveReceived = recv(getFileSystemSocket() , &header , sizeof(MpsHeader) , 0);

			}

		}

		info("Se termino de escribir el archivo");

		fclose(file1);
	}

