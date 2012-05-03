/*
 * kss-core-services.h
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_CORE_SERVICES_H_
#define KSS_CORE_SERVICES_H_

	void mountVda(char * vdaName , RuntimeErrorValidator * validator);
	void umountVda(char * vdaName , RuntimeErrorValidator * validator);
	List getPathContentElements(char * path , RuntimeErrorValidator * validator);
	void format(char * vdaName , RuntimeErrorValidator * validator);

#endif /* KSS_CORE_SERVICES_H_ */
