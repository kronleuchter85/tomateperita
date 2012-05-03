/*
 * kss-tdd.c
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <stdio.h>
#include "kss-tdd.h"
#include "linux-commons.h"
#include "linux-commons-list.h"


	void listingRecord(TddRecord * r);
	BOOL isTddRecordEqualsThan(TddRecord * t1 , TddRecord * t2);
	BOOL isTddRecordLessThan(TddRecord * t1 , TddRecord * t2);


	List tdd;


	/**************************** public functions ************************/

	void initializeTdd(){
		tdd = createList(
				&listingRecord ,
				&isTddRecordEqualsThan ,
				&isTddRecordLessThan);
	}

	TddRecord * createTddRecord(int fileHandleId , char * vdaName , int openingMode , int fileSize){
		TddRecord * t = malloc(sizeof(TddRecord));
		t->fileHandleId = fileHandleId;
		t->vdaName = vdaName;
		t->openingMode = openingMode;
		t->fileSize = fileSize;
		return t;
	}

	void addTddRecord(TddRecord * r){
		addNode(tdd , r );
	}

	void removeTddRecord(TddRecord * r){
		removeNode(tdd , r );
	}

	void listTddRecords(){
		listNodes(tdd );
	}


	/*************************** private functions *********************/


	BOOL isTddRecordEqualsThan(TddRecord * t1 , TddRecord * t2){
		if(t1 != NULL && t2 != NULL)
			return (t1->fileHandleId = t2->fileHandleId );
		else if( t1==NULL && t2 == NULL)
			return TRUE;
		else
			return FALSE;
	}

	BOOL isTddRecordLessThan(TddRecord * t1 , TddRecord * t2){
		if(t1 != NULL && t2 != NULL)
			return (t1->fileHandleId < t2->fileHandleId );
		else if( t1==NULL && t2 == NULL)
			return TRUE;
		else
			return FALSE;
	}

	void listingRecord(TddRecord * r){
		printf("{fileHandleId: %i , vdaName: %s , openingMode: %i , fileSize: %i}\n" ,
				r->fileHandleId , r->vdaName , r->openingMode , r->fileSize);
	}

