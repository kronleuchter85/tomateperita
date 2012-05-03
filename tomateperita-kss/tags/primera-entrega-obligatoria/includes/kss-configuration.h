/*
 * configuration.h
 *
 *  Created on: 29/05/2011
 *      Author: gonzalo
 */


	char * getShellPort();
	void setShellPort(char * sp);

	char * getFtpsPort();
	void setFtpsPort(char * fp);

	void setVdaListenPort(char * v);
	char * getVdaListenPort();

	char * getFssListenPort();
	void setFssListenPort(char * l);

	void setupConfiguration();
