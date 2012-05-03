


	char * encodeHost(char * address , HeapHandler h);
	char * encodePort(char * port , HeapHandler h);
	char * calculatePort(char * port , HeapHandler);
	char * decodeHost(char * encoded , HeapHandler);
	char * decodePort(char * encoded , HeapHandler);

	char * parseHostAndPort(char * request , HeapHandler h);
	char * encodeHostAndPort(char * host , char * port , HeapHandler h);
	