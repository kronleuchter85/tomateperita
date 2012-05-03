/*
 * fss-kss-entrypoint-core.h
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */

#ifndef FSS_KSS_ENTRYPOINT_CORE_H_
#define FSS_KSS_ENTRYPOINT_CORE_H_

	List coreOperationFileInformation(char * vdaName, char * fileName ,
			RuntimeErrorValidator * validator);

	List coreOperationAssignAndGetSectors(char * vdaName , char * fileName ,
			RuntimeErrorValidator * validator);


#endif /* FSS_KSS_ENTRYPOINT_CORE_H_ */
