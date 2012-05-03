/*
 * kss-core-services.h
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */

#include "linux-commons-errors.h"
#include "linux-commons-file.h"

#include "kss-tdd.h"

#ifndef KSS_CORE_SERVICES_H_
#define KSS_CORE_SERVICES_H_

	void mountVda(char * vdaName , RuntimeErrorValidator * validator);
	void umountVda(char * vdaName , RuntimeErrorValidator * validator);
	List getPathContentElements(char * path , RuntimeErrorValidator * validator);
	void format(char * vdaName , RuntimeErrorValidator * validator);
	int openFileInTdd(char * vdaName , char * fileName , char * openingMode , RuntimeErrorValidator * validator);

	void flush(TddRecord * record , RuntimeErrorValidator * validator);




#endif /* KSS_CORE_SERVICES_H_ */
