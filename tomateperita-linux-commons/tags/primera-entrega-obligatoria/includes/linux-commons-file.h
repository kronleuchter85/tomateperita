/*
 *
 *  file-config.h
 *
 *
 */
/*
 char * getClientIp(void);
 int getClientPort(void);
 int getLoggingLevel(void);
 */

	char * getKey(char * line);
	char * getValue(char * line);
	void loadConfiguration(
			char * configFile ,
			void (*processKeysAndValues)(char * , char *));
