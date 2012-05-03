/*
 * kss-tdd.c
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <stdio.h>

#include "linux-commons.h"
#include "linux-commons-list.h"

#include "kss-core-services.h"
#include "kss-tdd.h"
#include "kss-utils.h"



	/*
	 * Mutext para operaciones atomicas de agregar un registro
	 * y asignar su filehandleid correspondiente sin que otro
	 * thread este intentando agregar otro registro diferente
	 * bajo el mismo filehandleid.
	 */
	ThreadMutex tddMutex = PTHREAD_MUTEX_INITIALIZER;

	/*
	 * TDD - lista de todos los archivos abiertos
	 *
	 */
	List tdd;



	/*
	 * Inicializacion de la tdd
	 */

	void setTdd(List tddList){
		tdd = tddList;
	}
	List getTdd(){
		return tdd;
	}

	void initializeTdd(){

		void listing(TddRecord * tdd){

			char * fh = itoa(tdd->fileHandleId);
			char * sectors = serializeList(tdd->fileSectors , ",");
			char * fileSize = itoa(tdd->fileSize);
			char * opMode = itoa(tdd->openingMode);
			char * message = NULL;
			if(sectors == NULL){
				message = concatAll(10, "handle: " , fh , ", path: /"
						, tdd->vdaName , "/" , tdd->fileName , " , mode: " , opMode
						, " , size: " , fileSize  );
			}else{
				message = concatAll(12, "handle: " , fh , ", path: /"
						, tdd->vdaName , "/" , tdd->fileName , " , mode: " , opMode
						, " , size: " , fileSize , ", sectors: " , sectors );
			}
			debug(concatAll(2 , "Tdd Record: " , message));
		}
		Bool areEquals(TddRecord * t1 , TddRecord * t2){
			if(t1 == NULL || t2 == NULL)
				return FALSE;
			return equalsStrings(t1->fileName , t2->fileName )
					&& equalsStrings(t1->vdaName , t2->vdaName);
		}
		Bool isLess(TddRecord * t1 , TddRecord * t2){
			return t1->fileHandleId < t2->fileHandleId;
		}

		setTdd(createList((void (*)(void*))&listing ,
				(Bool (*)(void *, void *))&areEquals ,
				(Bool (*)(void *, void *))&isLess));

	}






	/*
	 * Builders de registros de tdd
	 */

	TddRecord * buildTddRecord(char * fileName , char * vdaName , char * openingMode ){
		TddRecord * t = malloc(sizeof(TddRecord));
		t->fileName = fileName;
		t->vdaName = vdaName;
		t->openingMode = getOpeningMode(openingMode);
		t->fileBlock = buildEmptyFileBlock();
		return t;
	}
	TddRecord * buildTddRecordWithSectors(char * fileName , char * vdaName , char * openingMode , List sectors){
		TddRecord * record = buildTddRecord(fileName , vdaName , openingMode);
		record->fileSectors = sectors;
		return record;
	}

	int getNextFileHandleId(){
		if(tdd == NULL)
			return 0;

		return tdd->size + 1;
	}




	/*
	 * Gestion de los registros de la tdd
	 * - Agregacion
	 * - Eliminacion
	 */

	void addTddRecord(TddRecord * r){
		/*
		 * Aca hay un doble lockeo porq la lista ya tiene accesos
		 * sincronizados. Si hay un deadlock mirar primero aca !!
		 */
		lockThreadMutex(&tddMutex);

		r->fileHandleId = getNextFileHandleId();
		addNode(tdd , r );

		unlockThreadMutex(&tddMutex);
	}

	void removeTddRecord(TddRecord * r){
		removeNode(tdd , r );
	}

	TddRecord * getTddRecordByFileHandleId(int fhId){
		Bool selection(TddRecord * r){
			return (r->fileHandleId == fhId);
		}
		return getNodeByCriteria(tdd , (Bool (*) (void * r)) selection);
	}









	/*
	 * - Gestion del estado de apertura de archivos
	 * - Estados de apertura
	 * - Traduccion entre el estado de apertura que llega via MPS al
	 * estado de apertura q maneja la tdd
	 */

	Bool isCurrentlyOpen(char * fileName , char * vdaName){
		return containsNode(tdd , buildTddRecord(fileName , vdaName , "dummy"));
	}

	int getOpeningMode(char * op){
		if(isOpeningModeRead(op)){
			return OPENING_MODE_READ;
		}else if(isOpeningModeWrite(op)){
			return OPENING_MODE_WRITE;
		}else if(isOpeningModeDelete(op)){
			return OPENING_MODE_DELETE;
		}else{
			return -1;
		}
	}

	Bool isOpeningModeWrite(char * op){
		return equalsStrings(KSS_FILE_OPENING_MODE_WRITE , op);
	}
	Bool isOpeningModeRead(char * op){
		return equalsStrings(KSS_FILE_OPENING_MODE_READ , op);
	}
	Bool isOpeningModeDelete(char * op){
		return equalsStrings(KSS_FILE_OPENING_MODE_DELETE , op);
	}

