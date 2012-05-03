/*
 * kss-ftps-entrypoint-core.c
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */


#include <stdlib.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-list.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-logging.h"

#include "kss-core-services.h"
#include "kss-fss-endpoint.h"
#include "kss-tdd.h"
#include "kss-utils.h"
#include "kss-vda-endpoint.h"
#include "kss-tdd.h"
#include "kss-vda-state.h"
#include "kss-ftps-entrypoint-core.h"



	char * coreOperationOpenFile(char * path , char * openingMode, RuntimeErrorValidator * validator){
		Iterator * tokens = buildIterator(tokenize(path , '/'));
		char * vdaName = hasMoreElements(tokens) ? next(tokens) : NULL;
		char * fileName = hasMoreElements(tokens) ? next(tokens) : NULL;

		if( !isOpeningModeDelete(openingMode)
				&& !isOpeningModeRead(openingMode)
				&& !isOpeningModeWrite(openingMode)){
			setError(validator , "El parametro de modo de apertura de archivo no es valido");
			return NULL;
		}

		if(fileName == NULL || vdaName == NULL){
			setError(validator , "La ruta al archivo no tiene el formato valido");
			return NULL;
		}

		int hdId = openFileInTdd(vdaName , fileName , openingMode , validator);

		if(hasError(validator) || hdId < 0){
			return NULL;
		}else{
			return itoa(hdId);
		}
	}







	void coreOperationCloseFile(char * fhId , RuntimeErrorValidator * validator){
		if(fhId == NULL ){
			setError(validator , "El descriptor de archivo es nulo");
			return;
		}

		TddRecord * tddRecord = getTddRecordByFileHandleId(atoi(fhId));
		if( tddRecord == NULL ){
			setError(validator , "El archivo no se encuentra abierto");
			return;
		}

		if(tddRecord->openingMode == OPENING_MODE_DELETE){
			callFssOperationDeleteFile(tddRecord->vdaName , tddRecord->fileName , validator);
		}

		removeTddRecord(tddRecord);
	}









	char * coreOperationListFiles(char * pattern , RuntimeErrorValidator * validator){
		if(isDebugEnabled())
			debug(concatAll(2 , "Listando Ruta: " , pattern));

		List elements = getPathContentElements(pattern  , validator);
		return serializeList(elements , ",");
	}








	void coreOperationWriteFile(char * fileId , char * contentBlock , RuntimeErrorValidator * validator){
		TddRecord * record = getTddRecordByFileHandleId(atoi(fileId));

		if(record->openingMode != OPENING_MODE_WRITE){
			setError(validator , "El archivo no fue abierto para escritura");
			return;
		}

		setFileBlockContent(record->fileBlock , contentBlock);

		flush(record , validator);
	}







	FileBlock * coreOperationReadFile(char * fileId , char * lastSectorSended , RuntimeErrorValidator * validator){

		TddRecord * record = getTddRecordByFileHandleId(atoi(fileId));

		if(record == NULL){
			setError(validator , "El archivo no se encuentra abierto o cargado en la TDD");
			return NULL;
		}

		if(record->openingMode != OPENING_MODE_READ){
			setError(validator , "El archivo no esta abierto en modo solo lectura");
			return NULL;
		}

		Iterator * iteSectors = buildIterator(record->fileSectors);

		/*
		 * Si se ha hecho un envio antes y existe un indice a partir del cual buscar
		 * entonces se recorre hasta dicho indice.
		 * De lo contrario se utiliza el primer numero de sector como indice ya q no
		 * ha habido ninguno anterior.
		 */
		if(!equalsStrings(lastSectorSended , KSS_FTP_ENTRYPOINT_READ_FILE_NO_PREVIOUS_SEARCH_INDEX) ){
			while( hasMoreElements(iteSectors) && !equalsStrings(lastSectorSended , next(iteSectors)) );
		}

		int sector1ToSend = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;
		int sector2ToSend = hasMoreElements(iteSectors) ? atoi(next(iteSectors)) : -1;

		if(sector1ToSend < 0 || sector2ToSend < 0){
			setError(validator , "Los numeros de sectores encontrados son invalidos");
			return NULL;
		}

		VdaComponent * comp = getMountedVda(record->vdaName);

		if(comp == NULL){
			setError(validator , "El VDA no se encuentra montado");
			return NULL;
		}

		return callVdaOperationGetSectores(comp , sector1ToSend , sector2ToSend , validator);

	}


