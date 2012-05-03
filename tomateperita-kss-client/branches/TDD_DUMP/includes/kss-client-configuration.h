/*
 * kss-client-configuration.h
 *
 *  Created on: 05/06/2011
 *      Author: gonzalo
 */

#ifndef KSS_CLIENT_CONFIGURATION_H_
#define KSS_CLIENT_CONFIGURATION_H_


#define KSS_CLIENT_CONFIGURATION_FILE_NAME		"/home/guest/2011.1C.TomatePerita/tomateperita-kss-client/branches/TDD_DUMP/conf/tomateperita-kss-client.properties"
#define KSS_CLIENT_CONFIGURATION_KEY_KSS_HOST	"kss.configuration.kss.listen.host"
#define KSS_CLIENT_CONFIGURATION_KEY_KSS_PORT	"kss.configuration.kss.listen.port"


	void setKssServerHost(char * s);
	void setKssServerPort(char * s);
	char * getKssServerHost();
	char * getKssServerPort();
	void setupConfiguration();

#endif /* KSS_CLIENT_CONFIGURATION_H_ */
