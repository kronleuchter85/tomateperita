/*
 * configuration.h
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */
#ifndef KSS_CONFIGURATION_H
#define KSS_CONFIGURATION_H

#define KSS_CONFIGURATION_FILE_NAME			"/home/guest/2011.1C.TomatePerita/tomateperita-kss/branches/TDD_DUMP/conf/tomateperita-kss.properties"
#define KSS_CONFIGURATION_KEY_FTP_PORT		"kss.configuration.ftp.listen.port"
#define KSS_CONFIGURATION_KEY_FSS_PORT		"kss.configuration.fss.listen.port"
#define KSS_CONFIGURATION_KEY_VDA_PORT		"kss.configuration.vda.listen.port"
#define KSS_CONFIGURATION_KEY_SHELL_PORT	"kss.configuration.shell.listen.port"
#define KSS_CONFIGURATION_LOGGING_LEVEL		"kss.configuration.logging.level"

	char * getShellPort();
	void setShellPort(char * sp);

	char * getFtpsPort();
	void setFtpsPort(char * fp);

	void setVdaListenPort(char * v);
	char * getVdaListenPort();

	char * getFssListenPort();
	void setFssListenPort(char * l);

	void setupConfiguration();

#endif
