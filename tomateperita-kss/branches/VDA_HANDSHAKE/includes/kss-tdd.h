/*
 * kss-tdd.h
 *
 *  Created on: 02/06/2011
 *      Author: gonzalo
 */

#include "linux-commons-list.h"
#include "linux-commons-file.h"

#ifndef KSS_TDD_H_
#define KSS_TDD_H_

#define KSS_FILE_OPENING_MODE_READ					"0"
#define KSS_FILE_OPENING_MODE_WRITE 				"1"
#define KSS_FILE_OPENING_MODE_DELETE				"2"


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
		FileBlock * fileBlock;
		List fileSectors;

	} TddRecord;


	List getTdd();
	void setTdd(List tddList);

	void initializeTdd();
	void addTddRecord(TddRecord * r);
	void removeTddRecord(TddRecord * r);

	Bool isCurrentlyOpen(char * fileName , char * vdaName);
	int getOpeningMode(char * );
	int getNextFileHandleId();

	Bool isOpeningModeWrite(char * o);
	Bool isOpeningModeRead(char * r);
	Bool isOpeningModeDelete(char * d);

	TddRecord * getTddRecordByFileHandleId(int fhId);

	TddRecord * buildTddRecord(char * fileName , char * vdaName , char * openingMode );
	TddRecord * buildTddRecordWithSectors(char * fileName , char * vdaName , char * openingMode , List sectors);


#endif /* KSS_TDD_H_ */
