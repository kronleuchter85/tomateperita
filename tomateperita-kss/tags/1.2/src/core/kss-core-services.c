/*
 * kss-core-services.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */
#include "linux-errors.h"
#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "linux-commons-file.h"

#include "kss-fss-endpoint.h"
#include "kss-vda-endpoint.h"
#include "kss-vda-state.h"
#include "kss-utils.h"

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
