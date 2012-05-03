/*
 * kss-client-utils.h
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */

#ifndef KSS_CLIENT_UTILS_H_
#define KSS_CLIENT_UTILS_H_

	void info(char * message);
	void debug(char * message);
	void error(char * message);

	List getFirstParameter(char * message);

#endif /* KSS_CLIENT_UTILS_H_ */
