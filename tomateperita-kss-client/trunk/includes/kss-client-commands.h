/*
 * kss-client-commands.h
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_CLIENT_COMMANDS_H_
#define KSS_CLIENT_COMMANDS_H_


#define SHELL_COMMAND_MOUNT						"mount"
#define SHELL_COMMAND_UMOUNT					"umount"
#define SHELL_COMMAND_LIST							"ls"
#define SHELL_COMMAND_FORMAT						"format"
#define SHELL_COMMAND_TDD_DUMP				"tdd_dump"
#define SHELL_COMMAND_MD5SUM					"md5sum"
#define SHELL_COMMAND_DISCONNECT				"disconnect"
#define SHELL_COMMAND_QUIT							"quit"
#define SHELL_COMMAND_SHOW_DEVICES		"show_devices"


	Bool isCommandMount(char * );
	Bool isCommandUmount(char * );
	Bool isCommandFormat(char * );
	Bool isCommandList(char * );
	Bool isCommandTddDump(char * );
	Bool isCommandMd5Sum(char * );
	Bool isCommandDisconnect(char *);
	Bool isCommandQuit(char * );
	Bool isCommandShowDevices(char *);

#endif /* KSS_CLIENT_COMMANDS_H_ */
