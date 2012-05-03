/*
 * kss-tdd.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"

#ifndef KSS_TDD_H_
#define KSS_TDD_H_

	enum OpeningMode {
		OPENING_MODE_READ , OPENING_MODE_WRITE , OPENING_MODE_DELETE
	};

	typedef struct {

	} FileRecord;

	typedef struct {

		int fileHandleId;
		char * fileName;
		char * vdaName;
		int openingMode;
		int fileSize;
		char fileBuffer[1024];
		List fileRecords;

	} TddRecord;


	void initializeTdd();
	void addTddRecord(TddRecord * r);
	void removeTddRecord(TddRecord * r);

	TddRecord * createTddRecord(int fileHandleId , char * vdaName ,
			int openingMode , int fileSize);

#endif /* KSS_TDD_H_ */
