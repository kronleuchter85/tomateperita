#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons-list.h"
#include "windows-commons.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"



	void callKssOperationCloseFile(int handleId , RuntimeErrorValidator * validator);

	int callKssOperationOpenFile(char * path , int mode , RuntimeErrorValidator * validator);

	List callKssOperationList(char * path , RuntimeErrorValidator * validator);

	void callKssOperationWrite(int handleId, FileBlock * block);

	FileBlock * callKssOperationRead(int handleId);

