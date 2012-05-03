/*
 * kss-client-configuration.h
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#ifndef KSS_CLIENT_CONFIGURATION_H_
#define KSS_CLIENT_CONFIGURATION_H_


	void setKssServerHost(char * s);
	void setKssServerPort(char * s);
	char * getKssServerHost();
	char * getKssServerPort();
	void setupConfiguration();

#endif /* KSS_CLIENT_CONFIGURATION_H_ */
