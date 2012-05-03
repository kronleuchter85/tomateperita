
#include "memory-commons.h"

#ifndef FILE_TRANSFER_CHUNK
#define FILE_TRANSFER_CHUNK		1024
#endif

#ifndef MAX_CONCATENATION_LENTH
#define MAX_CONCATENATION_LENTH	2048
#endif

#ifndef COMMON_KEY_LEN
#define COMMON_KEY_LEN	256
#endif


	int equalsStrings(char * s1 , char * s2);
	char * newString(HeapHandler h , char * content , int size);
	char * concatString(char * a , char * b , HeapHandler hh);
	char * concat(HeapHandler hh , int n , ...);
	int numdigits(int n);
	char * intToString(int n , HeapHandler hh);
	BOOL startsWith(char * s1 , char * s2);

	char * replaceString(char * string , char replaced , char replacing , HeapHandler h);

	char * subString(char * string , int startIndex , int endIndex , HeapHandler h);
	int indexOfString(char * s , char * b);	
	void blockMutex(HANDLE mutex);
	void unblockMutex(HANDLE mutex);

	char * trim(char * string , HeapHandler h);
	char * trimBlankSpaces(char * string , HeapHandler h);
	char * trimNewLine(char * string , HeapHandler h);
	char * trimAll(char * string , HeapHandler h);
	int indexOfChar(char * string , char c);

	
	char * serializeInt(int v , int spacesToFix , HeapHandler hh);
	char * randomBytes(int size , HeapHandler hh);
	char * generateRandomKey(int size , HeapHandler hh);
	char * replaceAll(char *buf , int *sizeBuf, char *oldPattern, int sizeOldPat, char *newPattern, int sizeNewPat, HeapHandler hh);