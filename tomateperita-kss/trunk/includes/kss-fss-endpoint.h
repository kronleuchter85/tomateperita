/*
 * kss-fss-endpoint.h
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-errors.h"
#include "linux-commons.h"
#include "linux-commons-file.h"

#ifndef KSS_FSS_OPERATIONS_H_
#define KSS_FSS_OPERATIONS_H_

	Bool callFssOperationIsExistingFile(char * vdaName , char * fileName , RuntimeErrorValidator * v);
	FileInformation * callFssOperationGetFileInformation(char * vdaName , char * fileName , RuntimeErrorValidator * v);
	void callFssOperationCreateFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator);
	void callFssOperationDeleteFile(char * vdaName , char * fileName , RuntimeErrorValidator * validator);
	void callFssOperationFormat(char * vdaName , int sectorsCount , RuntimeErrorValidator * validator);

	List callFssOperationAssignAndGetSectors(char * vdaName , char * filename , RuntimeErrorValidator * validator);
	List callFssOperationGetDirectoryInformation(char * vdaName , RuntimeErrorValidator * validator);


	Bool callFssOperationHasVdaFormat(char * vdaName , RuntimeErrorValidator * validator);
	void callFssOperationUpdateFileSize(char * vdaName , char * fileName , long size , RuntimeErrorValidator * validator);

#endif /* KSS_FSS_OPERATIONS_H_ */