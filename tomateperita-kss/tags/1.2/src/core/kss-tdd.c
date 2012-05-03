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

#include "kss-tdd.h"

	List tdd;

	void initializeTdd(){

		Bool areEquals(TddRecord * t1 , TddRecord * t2){
			if(t1 == NULL || t2 == NULL)
				return FALSE;
			return equalsStrings(t1->fileName , t2->fileName );
		}
		Bool isLess(TddRecord * t1 , TddRecord * t2){
			return TRUE;
		}

		tdd = createList(NULL ,
				(Bool (*)(void *, void *))&areEquals ,
				(Bool (*)(void *, void *))&isLess);
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

