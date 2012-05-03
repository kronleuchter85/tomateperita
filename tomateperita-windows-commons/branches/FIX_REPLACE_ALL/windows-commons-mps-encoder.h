/*
 * windows-commons-mps-encoder.h
 *
 *  Created on: 17/07/2011
 *      Author: gonzalo
 *	Modified on: 27/07/2011 by javier
 */

#ifndef LINUX_COMMONS_MPS_ENCODER_H_
#define LINUX_COMMONS_MPS_ENCODER_H_

#include "memory-commons.h"

#define MPS_ENCODING_CHARACTER_ACCENT_A			"\xE1"
#define MPS_ENCODING_CHARACTER_ACCENT_E			"\xE9"
#define MPS_ENCODING_CHARACTER_ACCENT_I			"\xED"
#define MPS_ENCODING_CHARACTER_ACCENT_O			"\xF3"
#define MPS_ENCODING_CHARACTER_ACCENT_U			"\xFA"
#define MPS_ENCODING_CHARACTER_CLCR				"\n"
#define MPS_ENCODING_CHARACTER_QUOTES			"\""
#define MPS_ENCODING_CHARACTER_SPACE			" "

#define MPS_ENCODED_CHARACTER_ACCENT_A			"{%ACA%}"
#define MPS_ENCODED_CHARACTER_ACCENT_E			"{%ACE%}"
#define MPS_ENCODED_CHARACTER_ACCENT_I			"{%ACI%}"
#define MPS_ENCODED_CHARACTER_ACCENT_O			"{%ACO%}"
#define MPS_ENCODED_CHARACTER_ACCENT_U			"{%acU%}"
#define MPS_ENCODED_CHARACTER_CLCR				"{%CLCR%}"
#define MPS_ENCODED_CHARACTER_QUOTES			"{%CQ%}"
#define MPS_ENCODED_CHARACTER_SPACE				"{%S%}"


	char * encode(char * buf, int *bufSize, HeapHandler hh);
	char * decode(char * buf, int *bufSize, HeapHandler hh);


#endif /* WINDOWS_COMMONS_MPS_ENCODER_H_ */
