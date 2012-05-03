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

#include "kss-utils.h"

	FileBlock * getFileBlock(char * buffer){
		FileBlock * block = buildEmptyFileBlock();
		memcpy(block->sector1.data , buffer , 512);
		memcpy(block->sector2.data , buffer + 512 , 512);
		return block;
	}


	void testFileTransfer(){

		MpsHeader header;

		header.payloadDescriptor = '0';
		memcpy(header.descriptorId , generateRandomKey(16) , 16);
		//strcpy(header.descriptorId , generateRandomKey(16));

		File * file1 = fopen("/home/gonzalo/images/anonimo.jpg" , "r");

		char readingBuffer[1024];
		char buffer[65536];

		char * opName = "snd_img1";

		int offset = 0;
		while(! feof(file1) && header.payloadDescriptor == '0'){

			bzero(buffer , 65536);
			fread(readingBuffer , sizeof(char) , 1024 , file1);

			FileBlock * block = getFileBlock(readingBuffer);
			header.payloadDescriptor = 'b';
			header.payloadSize = sizeof(FileBlock) + strlen("snd_img1");

			memcpy(buffer , &header , sizeof(MpsHeader));
			offset += sizeof(MpsHeader);
			memcpy(buffer + 21 , opName , strlen("snd_img1"));
			offset += strlen("snd_img1");
			memcpy(buffer + 29 , block , sizeof(FileBlock));
			offset += sizeof(FileBlock);

			sendBytes(getFileSystemConnection() , buffer , offset );


			recv(getFileSystemConnection() , &header , sizeof(MpsHeader) , 0);
		}

		header.payloadSize = 0;
		header.payloadDescriptor = 'a';
		sendBytes(getFileSystemConnection() , &header , sizeof(MpsHeader));

		fclose(file1);

	}
