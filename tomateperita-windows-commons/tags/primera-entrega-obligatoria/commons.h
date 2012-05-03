
#include "memory-commons.h"





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
	int indexOfChar(char * string , char c);
