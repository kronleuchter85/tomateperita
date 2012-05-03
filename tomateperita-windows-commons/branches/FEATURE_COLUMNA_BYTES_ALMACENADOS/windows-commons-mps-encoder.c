/*
 * linux-commons-mps-encoder.c
 *
 *  Created on: 17/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "windows-commons-mps-encoder.h"
#include "windows-commons.h"

	char * encodeCharacterSpace(char * s){
		return replaceAll(s , MPS_ENCODING_CHARACTER_SPACE , MPS_ENCODED_CHARACTER_SPACE);
	}


	char * encodeCharacterCrCl(char * contenido1){
		return replaceAll(contenido1 , MPS_ENCODING_CHARACTER_CLCR , MPS_ENCODED_CHARACTER_CLCR);
	}

	char * encodeCharacterAcent(char * contenido1){
		contenido1 = replaceAll(contenido1 , MPS_ENCODING_CHARACTER_ACCENT_A , MPS_ENCODED_CHARACTER_ACCENT_A);
		contenido1 = replaceAll(contenido1 , MPS_ENCODING_CHARACTER_ACCENT_E , MPS_ENCODED_CHARACTER_ACCENT_E);
		contenido1 = replaceAll(contenido1 , MPS_ENCODING_CHARACTER_ACCENT_I , MPS_ENCODED_CHARACTER_ACCENT_I);
		contenido1 = replaceAll(contenido1 , MPS_ENCODING_CHARACTER_ACCENT_O , MPS_ENCODED_CHARACTER_ACCENT_O);
		contenido1 = replaceAll(contenido1 , MPS_ENCODING_CHARACTER_ACCENT_U , MPS_ENCODED_CHARACTER_ACCENT_U);
		return contenido1;
	}

	char * encodeCharacterQuote(char * contenido1){
		return replaceAll(contenido1 , MPS_ENCODING_CHARACTER_QUOTES , MPS_ENCODED_CHARACTER_QUOTES);
	}




	/*
	 *  ******************************* Funciones decodificadoras ***********************************
	 */

	char * decodeCharacterSpace(char * contenido1){
		return replaceAll(contenido1 , MPS_ENCODED_CHARACTER_SPACE , MPS_ENCODING_CHARACTER_SPACE);
	}

	char * decodeCharacterAcent(char * contenido1){
		contenido1 = replaceAll(contenido1 , MPS_ENCODED_CHARACTER_ACCENT_A, MPS_ENCODING_CHARACTER_ACCENT_A);
		contenido1 = replaceAll(contenido1 , MPS_ENCODED_CHARACTER_ACCENT_I, MPS_ENCODING_CHARACTER_ACCENT_I);
		contenido1 = replaceAll(contenido1 , MPS_ENCODED_CHARACTER_ACCENT_E, MPS_ENCODING_CHARACTER_ACCENT_E);
		contenido1 = replaceAll(contenido1 , MPS_ENCODED_CHARACTER_ACCENT_O, MPS_ENCODING_CHARACTER_ACCENT_O);
		contenido1 = replaceAll(contenido1 , MPS_ENCODED_CHARACTER_ACCENT_U, MPS_ENCODING_CHARACTER_ACCENT_U);

		return contenido1;
	}

	char * decodeCharacterQuote(char * contenido1){
		return replaceAll(contenido1 , MPS_ENCODED_CHARACTER_QUOTES , MPS_ENCODING_CHARACTER_QUOTES);
	}

	char * decodeCharacterCrcl(char * contenido1){
		return replaceAll(contenido1  , MPS_ENCODED_CHARACTER_CLCR, MPS_ENCODING_CHARACTER_CLCR);
	}


	/*
	 * *****************Interfaz de alto nivel de funciones de codificacion y decodificacion *******************
	 */

	char * encode(char * contenido1){
		
		contenido1 = encodeCharacterAcent(contenido1);		
		contenido1 = encodeCharacterSpace(contenido1);		
		contenido1 = encodeCharacterCrCl(contenido1);		
		contenido1 = encodeCharacterQuote(contenido1);

		return contenido1;
	}

	char * decode(char * contenido1){
		
		contenido1 = decodeCharacterAcent(contenido1);		
		contenido1 = decodeCharacterSpace(contenido1);		
		contenido1 = decodeCharacterCrcl(contenido1);		
		contenido1 = decodeCharacterQuote(contenido1);
		
		return contenido1;
	}
