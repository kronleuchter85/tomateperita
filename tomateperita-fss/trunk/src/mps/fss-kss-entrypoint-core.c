/*
 * fss-kss-entrypoint-core.c
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-remote-services.h"
#include "linux-commons-list.h"
#include "linux-commons-mps.h"

#include "fss-kss-entrypoint.h"
#include "fss-fs-management.h"
#include "fss-utils.h"






	List coreOperationFileInformation(char * vdaName, char * fileName , RuntimeErrorValidator * validator){

		FileInformation * info = getFileInformation(vdaName , fileName , validator);

		if(hasError(validator)){
			return NULL;
		}

		char * fileSize = ltoa(info->size);
		char * fileSectors = serializeList(info->sectors , ",");

		/*
		 *  Si el archivo no tiene sectores mandamos un null.
		 */
		if(fileSectors == NULL)
			fileSectors = MPS_FIELD_ARGUMENT_NULL;

		return buildStringsListWithValues(2 , fileSize , fileSectors);

	}




	List coreOperationAssignAndGetSectors(char * vdaName , char * fileName , RuntimeErrorValidator * validator){
		int firstSector = getFreeSector(vdaName , validator);
		if(hasError(validator)){
			return NULL;
		}

		int secondSector = getFreeSector(vdaName , validator);
		if(hasError(validator)){
			return NULL;
		}

		addSectorsToFile(vdaName , fileName , firstSector , secondSector , validator);

		if(hasError(validator)){
			return NULL;
		}

		return buildStringsListWithValues(1 ,
				serializeList(buildStringsListWithValues(2 , itoa(firstSector) , itoa(secondSector)) , ","));
	}

