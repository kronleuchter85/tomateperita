/*
 * kss-sys-calls.h
 *
 *  Created on: 05/08/2011
 *      Author: gonzalo
 */

#ifndef KSS_SYS_CALLS_H_
#define KSS_SYS_CALLS_H_
	void executeOperationRead(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationWrite(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationOpen(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationClose(ListenSocket , MpsMessage * aReq,RuntimeErrorValidator *);


#endif /* KSS_SYS_CALLS_H_ */
