#ifndef _WINDOWS_COMMONS_REMOTE_OPERATIONS
#define _WINDOWS_COMMONS_REMOTE_OPERATIONS

#define KSS_SYS_CALL_OPEN 				"kss_op01"
#define KSS_SYS_CALL_CLOSE 				"kss_op02"
#define KSS_SYS_CALL_READ 				"kss_op03"
#define KSS_SYS_CALL_WRITE				"kss_op04"
#define KSS_SYS_CALL_FLUSH				"kss_op05"
#define KSS_SYS_CALL_FLIST				"kss_op06"


#define FSS_FILE_CHECK_EXISTENCE		"fss_op01"
#define FSS_FILE_INFORMATION			"fss_op02"
#define FSS_FILE_CREATION				"fss_op04"
#define FSS_SECTORS_TABLE_CREATION		"fss_op06"
#define FSS_FILE_DELETION				"fss_op03"
#define FSS_DIRECTORY_LISTING			"fss_op10"
#define FSS_FORMAT						"fss_op11"
#define FSS_SECTORS_ASSIGN_AND_GET		"fss_op12"


#define VDA_SECTORS_GETTING				"vda_op01"
#define VDA_SECTORS_PUTTING				"vda_op02"
#define VDA_CHS_GETTING					"vda_op03"
#define VDA_IDENTIFY_NAME				"vda_op04"

#define SHELL_MOUNT						"shl_op01"
#define SHELL_UMOUNT					"shl_op02"
#define SHELL_FORMAT					"shl_op03"
#define SHELL_LS						"shl_op04"
#define SHELL_TDD_DUMP					"shl_op05"
#define SHELL_MD5_SUM					"shl_op06"


#endif