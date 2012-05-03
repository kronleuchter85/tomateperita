#include "windows-commons-socket.h"
#include "windows-commons-file.h"
#include "windows-commons-list.h"
#include "windows-commons.h"
#include "windows-commons-memory.h"
#include "windows-commons-errors.h"

#include "ftp-client-thread-resources.h"
#include "ftp-response-builder.h"
#include "ftp-commons.h"
#include "ftp.h"



	void callKssOperationCloseFile(HeapHandler hh, int handleId , RuntimeErrorValidator * validator);
	int callKssOperationOpenFile(HeapHandler hh, char * path , int mode , RuntimeErrorValidator * validator);
	List callKssOperationList(HeapHandler hh, char * path , RuntimeErrorValidator * validator);
	void callKssOperationWrite(HeapHandler hh,int handleId , FileBlock * block , RuntimeErrorValidator * validator);
	FileBlock * callKssOperationRead(HeapHandler hh);
	int callKssOperationStartReadingProcess(HeapHandler hh, char * path , RuntimeErrorValidator * validator);



	void doBusinessOperationWrite(HeapHandler hh, char * path , FileBlock * block , RuntimeErrorValidator * validator);