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
#include "kss-tdd.h"
#include "kss-vda-state.h"


	char * dest = "/home/gonzalo/tomateperita-data/imagen-restaurada-";
	char * imagePath = "/home/gonzalo/images/anonimo.jpg";
	int counter = 0;


	void integrateVdasWIthBlocks(){
		VdaComponent * comp = getConnectedVda("VDA3");

		RuntimeErrorValidator * validator = buildErrorSuccessValidator();

		File * file1 = fopen(imagePath , "r");

		char buffer1[512];
		char buffer2[512];

		int sector1 = 1;
		int sector2 = 2;

		while(! feof(file1) ){

			bzero(buffer1 , 512);
			bzero(buffer2 , 512);

			int readedS1 = fread(buffer1, sizeof(char) , 512 , file1);
			int readedS2 = fread(buffer2, sizeof(char) , 512 , file1);

			info(concatAll(5 , "Enviando contenido: { Sector1 = " , itoa(readedS1) , " , Sector2 = " , itoa(readedS2) , "}"));

			FileBlock * block = buildEmptyFileBlock();
			setFileBlockSectors(block , sector1 , sector2);
			setFileBlockContents(block , buffer1 , buffer2 , readedS1 , readedS2);

			callVdaOperationPutSectores(comp , block , validator);

			sector1 += 2;
			sector2 += 2;
		}

		fclose(file1);


		int s1 = 1;
		int s2 = 2;

		dest = concatAll(3 , dest , itoa(counter) , ".jpg");
		file1 = fopen(dest , "w");

		while(sector1 != s1 && sector2 != s2){

			FileBlock * block = callVdaOperationGetSectores(comp , s1 , s2 , validator);

			int written1 = fwrite( block->sector1.data, sizeof(char) , block->sector1.dataLength , file1);
			int written2 = fwrite( block->sector2.data, sizeof(char) , block->sector2.dataLength , file1);

			printf("Restaurados %i , %i \n" , written1 , written2);

			s1 += 2;
			s2 += 2;
		}

		fclose(file1);

	}


