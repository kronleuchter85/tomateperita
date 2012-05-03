/*
 * linux-commons-mps-encoder.h
 *
 *  Created on: 17/07/2011
 *      Author: gonzalo
 */

#ifndef LINUX_COMMONS_MPS_ENCODER_H_
#define LINUX_COMMONS_MPS_ENCODER_H_

#define MPS_ENCODING_CHARACTER_ACCENT_A			"á"
#define MPS_ENCODING_CHARACTER_ACCENT_E			"é"
#define MPS_ENCODING_CHARACTER_ACCENT_I			"í"
#define MPS_ENCODING_CHARACTER_ACCENT_O			"ó"
#define MPS_ENCODING_CHARACTER_ACCENT_U			"ú"
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



	char * encode(char * contenido1);
	char * decode(char * contenido1);


#endif /* LINUX_COMMONS_MPS_ENCODER_H_ */
