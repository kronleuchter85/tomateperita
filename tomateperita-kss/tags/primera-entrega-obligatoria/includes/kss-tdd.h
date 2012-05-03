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
		OPENING_MODEREAD , OPENING_MODE_WRITE
	};

	typedef struct {

	} FileRecord;

	typedef struct {

		int fileHandleId;
		char * vdaName;
		int openingMode;
		int fileSize;
		char fileBuffer[1024];
		List fileRecords;

	} TddRecord;


	void initializeTdd();
	void addTddRecord(TddRecord * r);
	void removeTddRecord(TddRecord * r);
	void listTddRecords();

	TddRecord * createTddRecord(int fileHandleId , char * vdaName ,
			int openingMode , int fileSize);

#endif /* KSS_TDD_H_ */
