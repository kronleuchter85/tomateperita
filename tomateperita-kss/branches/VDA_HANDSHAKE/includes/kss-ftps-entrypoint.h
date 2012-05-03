/*
 * kss-ftps-entrypoint.h
 *
 *  Created on: 22/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_FTPS_ENTRYPOINT_H_
#define KSS_FTPS_ENTRYPOINT_H_




	void executeOperationRead(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationWrite(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationOpen(ListenSocket , MpsMessage * aReq, RuntimeErrorValidator *);
	void executeOperationClose(ListenSocket , MpsMessage * aReq,RuntimeErrorValidator *);
	void executeOperationList(ListenSocket , MpsMessage * aReq ,RuntimeErrorValidator *);




#endif /* KSS_FTPS_ENTRYPOINT_H_ */
