#include "headers-commons.h"
#include "memory-commons.h"


#define BUFFSIZE 512



	int equalsStrings(char * s1 , char * s2);
	char * newString(HeapHandler h , char * content , int size);
	LPVOID getErrorMessage(void);
	int logger(TCHAR *procName[],DWORD,DWORD,char *,char *);