/*
 * utils.h
 *
 *  Created on: 14/06/2011
 *      Author: gonzalo
 */
#include "linux-commons-list.h"
#include "linux-commons-socket.h"
#include "linux-commons-errors.h"


#ifndef UTILS_H_
#define UTILS_H_

	void info(char * m);
	void debug(char * m);
	void error(char * m);


	List createVdasList();

	void doHandshake(ListenSocket l , char  ,RuntimeErrorValidator * );
#endif /* UTILS_H_ */
