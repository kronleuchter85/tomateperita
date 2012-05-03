#ifndef PARSERCONSOLA_H_
#define PARSERCONSOLA_H_

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER_LINE 100
#define MAX_SYMBOLS_COUNT 10

	typedef enum {

		CHARACTERS_SEQUENCE,

		ASCII_CHARACTER_40,

		ASCII_CHARACTER_44,

		ASCII_CHARACTER_41,

		E_NCQM_64

	} SyntaxToken;

	SyntaxToken nextToken(char * string);
	SyntaxToken scan(char * aString);

	int parseSequence(char * aString);

	void addToBuffer(char *buffer, char c);	
	void matchArguments(char *cadena ); 
	void match(SyntaxToken t , char * aString );
	void addSymbol(char * buffer);
	void removeAllSymbols();

	char * getSymbol(int i );

	int getSymbolsCount();
#endif