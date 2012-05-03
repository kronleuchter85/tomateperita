/*
 * fss-configuration.h
 *
 *  Created on: 22/06/2011
 *      Author: gonzalo
 */

#ifndef FSS_CONFIGURATION_H_
#define FSS_CONFIGURATION_H_

#define FSS_CONFIGURATION_FILE_NAME			"/home/gonzalo/workspace/tomateperita/tomateperita-fss/conf/tomateperita-fss.properties"
#define FSS_CONFIGURATION_KSS_HOST			"fss.configuration.kss.host"
#define FSS_CONFIGURATION_KSS_PORT			"fss.configuration.kss.port"
#define FSS_CONFIGURATION_LOGGING_LEVEL		"fss.configuration.logging.level"
#define FSS_CONFIGURATION_DATA_DIRECTORY	"fss.configuration.data.directory"


	void setKssHost(char * p);
	void setKssPort(char * p);
	char * getKssPort();
	char * getKssHost();
	void setupConfiguration();
	char * getDataDirectory();
	void setDataDirectory(char * d);

#endif /* FSS_CONFIGURATION_H_ */
