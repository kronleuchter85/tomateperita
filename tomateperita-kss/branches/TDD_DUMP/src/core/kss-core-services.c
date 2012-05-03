/*
 * kss-core-services.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <string.h>

#include "linux-commons-errors.h"
#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "linux-commons-file.h"

#include "kss-core-services.h"
#include "kss-fss-endpoint.h"
#include "kss-vda-endpoint.h"
#include "kss-vda-state.h"
#include "kss-utils.h"
#include "kss-tdd.h"


	Bool hasTwoSectors(FileBlock * fileBlock){
		if(fileBlock == NULL)
			return FALSE;

		if(strlen(fileBlock->sector2.data) == 0 || strlen(fileBlock->sector1.data) == 0)
			return FALSE;
		else
			return TRUE;
	}

	FileBlock * buildEmptyFileBlock(){
		FileBlock * fb = (FileBlock *)malloc(sizeof(FileBlock));
		cleanBlock(fb);
		return fb;
	}

	void setFileBlockSectors(FileBlock * block, int s1 , int s2){
		if(block == NULL)
			return;

		block->sector1.sector = s1;
		block->sector2.sector = s2;
	}

	void cleanBlock(FileBlock * block){
		if(block == NULL)
			return;

		setFileBlockSectors( block, -1, -1);
		bzero(block->sector1.data , sizeof(block->sector1.data ));
		bzero(block->sector2.data , sizeof(block->sector2.data ));
	}


	void setFileBlockContent(FileBlock * block , char * buffer){
		if(block == NULL)
			return;

		memcpy(block->sector1.data , buffer , sizeof(block->sector1.data));
		memcpy(block->sector2.data , buffer + sizeof(block->sector1.data) , sizeof(block->sector2.data));
	}

	void setFileBlockContents(FileBlock * block , char * content1 , char * content2){

		if(block == NULL)
			return;

		memcpy(block->sector1.data , content1 , strlen(content1));
		memcpy(block->sector2.data , content2 , strlen(content2));
	}


	void mountVda(char * vdaName , RuntimeErrorValidator * validator){
		VdaComponent * vda = getConnectedVda(vdaName);
		if(vda != NULL){
			removeConnectedVda(vda);
			addMountedVda(vda);
		}else{
			setError(validator , "El vda no esta conectado, puede que este montado o que no exista");
		}
	}



	void umountVda(char * vdaName , RuntimeErrorValidator * validator){
		VdaComponent * vda = getMountedVda(vdaName);
		if(vda != NULL){
			removeMountedVda(vda);
			addConnectedVda(vda);
		}else{
			setError(validator , "El vda no esta montado");
		}
	}




	List getVdaFiles(char * vda , RuntimeErrorValidator * validator){
		List fileNames = buildSringsList();
		List fileInformations = callFssOperationGetDirectoryInformation(vda , validator);
		Iterator * fileInfosIterator = buildIterator(fileInformations);

		while(hasMoreElements(fileInfosIterator)){
			FileInformation * infor = next(fileInfosIterator);
			addNode(fileNames , infor->name);
		}
		return (fileNames->size > 0) ? fileNames : NULL;
	}




	List getPathContentElements(char * path , RuntimeErrorValidator * validator){
		if(equalsStrings(path , "/")){
			return getVdasMountedNames();
		}else{
			return getVdaFiles(path , validator);
		}
	}



	int getVdaMaxSectorsCount(VdaInformation * infor){
		if(infor == NULL)
			return 0;

		return infor->headCounts * infor->sectorsCount * infor->tracksCount;
	}


	void format(char * vdaName , RuntimeErrorValidator * validator){

		if(vdaName == NULL || equalsStrings(trim(vdaName) , "")){
			setError(validator , "El nombre del dispositivo no es valido");
			return ;
		}

		VdaComponent * comp = getConnectedVda(vdaName);

		if(comp == NULL){
			comp = getMountedVda(vdaName);
		}

		if(comp == NULL){
			setError(validator , "El VDA que se quiere formatear no existe");
			return;
		}

		VdaInformation * vdainfo = callVdaOperationGetChs(comp , validator);

		int sectorsCount = getVdaMaxSectorsCount(vdainfo);

		callFssOperationFormat(comp->vdaName , sectorsCount , validator);
	}




	int openFileInTdd(char * vdaName , char * fileName , char * openingMode , RuntimeErrorValidator * validator){

		TddRecord * tddRecord = NULL;

		if(isCurrentlyOpen(fileName , vdaName)){
			setError(validator , "El archivo ya se encuentra abierto");
			return -1;
		}

		if(isOpeningModeRead(openingMode)){

			FileInformation * fileInfo = callFssOperationGetFileInformation(vdaName , fileName , validator);
			if(hasError(validator)){
				return -1;
			}

			tddRecord = buildTddRecordWithSectors(fileName , vdaName , openingMode , fileInfo->sectors);

		}else if(isOpeningModeWrite(openingMode)){

			if(callFssOperationIsExistingFile(vdaName , fileName , validator)){

				callFssOperationDeleteFile(vdaName , fileName , validator);
				if(hasError(validator)){
					return -1;
				}
			}

			callFssOperationCreateFile(vdaName , fileName , validator);
			if(hasError(validator)){
				return -1;
			}

			tddRecord = buildTddRecord(fileName , vdaName , openingMode );

		} else if( isOpeningModeDelete(openingMode)){

			tddRecord = buildTddRecord(fileName , vdaName , openingMode );
		}

		addTddRecord(tddRecord);

		return tddRecord->fileHandleId;
	}



	void flush(TddRecord * record , RuntimeErrorValidator * validator){


		if(!hasTwoSectors(record->fileBlock)){
			error(concatAll(4, "Ojo! Se estan asignando dos sectores para un bloque que solo necesita uno: /"
					, record->vdaName , "/" , record->fileName));
		}

		if(record->openingMode != OPENING_MODE_WRITE){
			setError(validator , "El archivo no fue abierto para escritura");
			return;
		}

		List sectors = callFssOperationAssignAndGetSectors(record->vdaName , record->fileName , validator);

		if(hasError(validator)){
			return;
		}

		Iterator * ite = buildIterator(sectors);

		int sector1 = atoi( hasMoreElements(ite) ? next(ite) : "-1" );
		int sector2 = atoi( hasMoreElements(ite) ? next(ite) : "-1" );

		if(sector1 < 0 || sector2 < 0 || sector1 == sector2 ){
			setError(validator , "Los numeros de sectores deben ser mayores o iguales a cero y deben ser distintos");
			return;
		}

		setFileBlockSectors( record->fileBlock , sector1 , sector2);

		VdaComponent * comp = getMountedVda(record->vdaName);

		if(comp == NULL){
			setError(validator , "El vda necesario para realizar el flush no se encuentra montado");
			return;
		}

		callVdaOperationPutSectores(comp , record->fileBlock , validator);
	}

