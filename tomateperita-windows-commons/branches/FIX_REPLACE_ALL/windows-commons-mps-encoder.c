/*
 * windows-commons-mps-encoder.c
 *
 *  Created on: 17/07/2011
 *      Author: gonzalo
 *  Modified on: 27/07/2011 by javier
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "windows-commons-mps-encoder.h"
#include "commons.h"


	/*
	 * *****************Interfaz de alto nivel de funciones de codificacion y decodificacion *******************
	 */

	char * encode(char *buf, int *bufSize, HeapHandler hh){
		int newsize = *bufSize;
		char *b1, *b2;
		b1 = replaceAll(buf,&newsize, MPS_ENCODING_CHARACTER_SPACE    , (int)strlen(MPS_ENCODING_CHARACTER_SPACE)   , MPS_ENCODED_CHARACTER_SPACE   , (int)strlen(MPS_ENCODED_CHARACTER_SPACE)   , hh);
		b2 = replaceAll(b1, &newsize, MPS_ENCODING_CHARACTER_CLCR     , (int)strlen(MPS_ENCODING_CHARACTER_CLCR)    , MPS_ENCODED_CHARACTER_CLCR    , (int)strlen(MPS_ENCODED_CHARACTER_CLCR)    , hh);
		if ((b1!=buf) && (b2!=b1)) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODING_CHARACTER_ACCENT_A , (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_A), MPS_ENCODED_CHARACTER_ACCENT_A, (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_A), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODING_CHARACTER_ACCENT_E , (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_E), MPS_ENCODED_CHARACTER_ACCENT_E, (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_E), hh);
		if (b1!=b2) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODING_CHARACTER_ACCENT_I , (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_I), MPS_ENCODED_CHARACTER_ACCENT_I, (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_I), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODING_CHARACTER_ACCENT_O , (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_O), MPS_ENCODED_CHARACTER_ACCENT_O, (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_O), hh);
		if (b1!=b2) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODING_CHARACTER_ACCENT_U , (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_U), MPS_ENCODED_CHARACTER_ACCENT_U, (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_U), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODING_CHARACTER_QUOTES   , (int)strlen(MPS_ENCODING_CHARACTER_QUOTES)  , MPS_ENCODED_CHARACTER_QUOTES  , (int)strlen(MPS_ENCODED_CHARACTER_QUOTES)  , hh);
		if (b1!=b2) freeMemory(hh, b1);
		*bufSize = newsize;
		return b2;
	}

	char * decode(char *buf, int *bufSize, HeapHandler hh){
		int newsize = *bufSize;
		char *b1, *b2;
		b1 = replaceAll(buf,&newsize, MPS_ENCODED_CHARACTER_SPACE    , (int)strlen(MPS_ENCODED_CHARACTER_SPACE)   , MPS_ENCODING_CHARACTER_SPACE   , (int)strlen(MPS_ENCODING_CHARACTER_SPACE)  , hh);
		b2 = replaceAll(b1, &newsize, MPS_ENCODED_CHARACTER_CLCR     , (int)strlen(MPS_ENCODED_CHARACTER_CLCR)    , MPS_ENCODING_CHARACTER_CLCR    , (int)strlen(MPS_ENCODING_CHARACTER_CLCR)    , hh);
		if ((b1!=buf) && (b2!=b1)) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODED_CHARACTER_ACCENT_A , (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_A), MPS_ENCODING_CHARACTER_ACCENT_A, (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_A), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODED_CHARACTER_ACCENT_E , (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_E), MPS_ENCODING_CHARACTER_ACCENT_E, (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_E), hh);
		if (b1!=b2) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODED_CHARACTER_ACCENT_I , (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_I), MPS_ENCODING_CHARACTER_ACCENT_I, (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_I), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODED_CHARACTER_ACCENT_O , (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_O), MPS_ENCODING_CHARACTER_ACCENT_O, (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_O), hh);
		if (b1!=b2) freeMemory(hh, b1);
		b1 = replaceAll(b2, &newsize, MPS_ENCODED_CHARACTER_ACCENT_U , (int)strlen(MPS_ENCODED_CHARACTER_ACCENT_U), MPS_ENCODING_CHARACTER_ACCENT_U, (int)strlen(MPS_ENCODING_CHARACTER_ACCENT_U), hh);
		if (b1!=b2) freeMemory(hh, b2);
		b2 = replaceAll(b1, &newsize, MPS_ENCODED_CHARACTER_QUOTES   , (int)strlen(MPS_ENCODED_CHARACTER_QUOTES)  , MPS_ENCODING_CHARACTER_QUOTES  , (int)strlen(MPS_ENCODING_CHARACTER_QUOTES)  , hh);
		if (b1!=b2) freeMemory(hh, b1);
		*bufSize = newsize;
		return b2;
	}
