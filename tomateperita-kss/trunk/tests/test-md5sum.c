/*
 * test-md5sum.c
 *
 *  Created on: Aug 5, 2011
 *      Author: Fernando
 */


#include "stdlib.h"

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-logging.h"
#include "linux-commons-errors.h"
#include "linux-commons-remote-services.h"
#include "string.h"

#include "kss-core-services.h"
#include "kss-utils.h"
#include "kss-shell-service.h"
#include "kss-state.h"
#include "kss-sys-calls.h"
#include "kss-fss-endpoint.h"
#include "kss-vda-state.h"
#include "kss-vda-endpoint.h"


void testMd5Sum(){
	char sourceImage[40] = "/home/guest/images/anonimo.png";
	RuntimeErrorValidator * v = buildErrorSuccessValidator();
	char buffer1[512];
	char buffer2[512];

	/* SECCION OPEN_FILE */

	List params = buildStringsListWithValues( 2 , "1" , "/VDA1/imagen.png");
	Iterator * ite = buildIterator(params);
	char * openingMode = hasMoreElements(ite) ? next(ite) : NULL;
	char * path = hasMoreElements(ite) ? next(ite) : NULL;

	char * handleId = coreOperationOpenFile(path , openingMode , v);
	FILE * image = fopen(sourceImage , "r");
	TddRecord * record;
	long fileSize = 0;
	int sector1 = 1;
	int sector2 = 2;

	/* SECCION WRITE_FILE */

	while( ! feof(image) ){
		bzero(buffer1 , 512);
		bzero(buffer2 , 512);

		int readS1 = fread(buffer1 , sizeof(char) , 512 , image);
		int readS2 = fread(buffer2 , sizeof(char) , 512 , image);

		record = getTddRecordByFileHandleId( atoi(handleId) );

		List sectors = callFssOperationAssignAndGetSectors(record->vdaName , record->fileName , v);

		Iterator * iteSectors = buildIterator(sectors);

		//int sector1 = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;
		//int sector2 = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;

		FileBlock * block = buildEmptyFileBlock();
		block->fileHandleid = atoi(handleId);
		setFileBlockSectors(block , sector1 , sector2);
		setFileBlockContents(block , buffer1 , buffer2 , readS1 , readS2);

		callVdaOperationPutSectores(getMountedVda(record->vdaName) , block , v);

		int bytesToWrite = block->sector1.dataLength + block->sector2.dataLength;
		fileSize += bytesToWrite;
		record->fileSize += bytesToWrite;

		callFssOperationUpdateFileSize(record->vdaName , record->fileName , fileSize , v);
		if(hasError(v)){
				error(v->errorDescription);
				return;
		}
		sector1 += 2;
		sector2 +=2;
	}

	/* SECCION CLOSE_FILE */
	removeTddRecord(record);
}

void listDevicesTest(ListenSocket sock){
	VdaComponent * vda1;
	VdaComponent * vda2;

	vda1 = buildVdaComponent( "VDA1" , sock);
	vda2 = buildVdaComponent( "VDA2" , sock);
	addMountedVda(vda1);
	addMountedVda(vda2);
}
