/*
 * kss-fss-endpoint.h
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */

#include "linux-errors.h"
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

#endif /* KSS_FSS_OPERATIONS_H_ */
