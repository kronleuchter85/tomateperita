#include "vda-shell-parser.h"
#include <windows.h>

	int symbolsCount;
	int processingIndexPosition;

	BOOL syntaxError;
	BOOL continueReading;

	char symbols[MAX_SYMBOLS_COUNT][MAX_BUFFER_LINE]; 
	
	const char transitions[][6] = {
		{1,3,4,5,0,6},
		{1,2,2,2,2,2},
		{99,99,99,99,99,99},
		{99,99,99,99,99,99},
		{99,99,99,99,99,99},
		{99,99,99,99,99,99},
		{99,99,99,99,99,99}
	}; 

	char getTransition(int x , int y){
		return transitions[x][y];
	}

	void putSymbol(int i , char * symbol){
		//strcpy(symbols[i], symbol);
		strcpy_s(symbols[i], MAX_BUFFER_LINE, symbol);
	}
	
	char * getSymbol(int i ){
		return symbols[i];
	}


	void setSyntaxError(BOOL s){
		syntaxError = s;
	}
	void setContinueReading(BOOL r){
		continueReading = r;
	}
	BOOL isSyntaxError(){
		return syntaxError;
	}
	BOOL isContinueReading(){
		return continueReading;
	}

	void setProcessingIndexPosition(int index){
		processingIndexPosition = index;
	}
	int getProcessingIndexPosition(){
		return processingIndexPosition;
	}
	void incrementProcessingIndexPosition(){
		processingIndexPosition++;
	}

	int getSymbolsCount(){
		return symbolsCount;
	}
	void setSymbolsCount(int i){
		symbolsCount = i;
	}
	void incrementSymbolsCount(){
		symbolsCount++;
	}



	SyntaxToken nextToken(char *cadena){
		static SyntaxToken tokenLeido;
		if(isContinueReading()){
			tokenLeido = scan(cadena);
			setContinueReading(FALSE);
		}
		return tokenLeido;
	}

	void match(SyntaxToken t, char *cadena){
		SyntaxToken tokenLeido = nextToken(cadena); 
		setContinueReading(TRUE);
		if(t != tokenLeido)
			setSyntaxError(TRUE);
	}

	void addToBuffer(char * buffer, char c){
		int i; 
		for(i=0; buffer[i]!='\0'; i++);
		buffer[i] = c;
		buffer[i+1] = '\0';
	}

	void addSymbol(char *buffer){
		int i;
		for(i=0; symbols[i][0]!='\0' && i<MAX_SYMBOLS_COUNT; i++); 
		if(i<MAX_SYMBOLS_COUNT){
			putSymbol(i , buffer);					
			incrementSymbolsCount();
		}
	}

	void removeAllSymbols(){
		int i;
		for(i=0; i<MAX_SYMBOLS_COUNT; i++)
			symbols[i][0] = '\0';
	}



	int parseSequence(char *cadena) {
		setProcessingIndexPosition(0);
		setSyntaxError(FALSE);
		setContinueReading(TRUE);
		setSymbolsCount(0);
		removeAllSymbols();

		match(CHARACTERS_SEQUENCE, cadena);
		match(ASCII_CHARACTER_40, cadena);
		if(nextToken(cadena)!=ASCII_CHARACTER_41)
			matchArguments(cadena);
		match(ASCII_CHARACTER_41, cadena);
		match(E_NCQM_64, cadena);
		
		if(isSyntaxError())
			return 1;
		else
			return 0;
	}

	void matchArguments(char *cadena){
		match(CHARACTERS_SEQUENCE, cadena);
		while(1){
			switch(nextToken(cadena)){
			case ASCII_CHARACTER_44:
				match(ASCII_CHARACTER_44, cadena);
				match(CHARACTERS_SEQUENCE, cadena);
				break;
			default:
				return;
			}
		}
	}

	SyntaxToken scan(char *cadena){
		char estado = 0;
		char c;
		char columna;
		char buffer[MAX_BUFFER_LINE] = "";
		
		while(1){
			c = cadena[getProcessingIndexPosition()];
			if(c == '(')
				columna = 1;
			else if(c == ',')
				columna = 2;
			else if(c == ')')
				columna = 3;
			else if(isspace(c))
				columna = 4;
			else if(c == '\0')
				columna = 5;
			else
				columna = 0;
			estado = getTransition(estado , columna);

			switch(estado){
				case 0: 
					incrementProcessingIndexPosition();
					break;
				case 1:
					addToBuffer(buffer, c); 
					incrementProcessingIndexPosition();
					break;
				case 2:
					addSymbol(buffer);
					return CHARACTERS_SEQUENCE;
				case 3:
					incrementProcessingIndexPosition();
					return ASCII_CHARACTER_40;
				case 4:
					incrementProcessingIndexPosition();
					return ASCII_CHARACTER_44;
				case 5:
					incrementProcessingIndexPosition();
					return ASCII_CHARACTER_41;
				case 6:
					return E_NCQM_64;
				}
		}
	}
