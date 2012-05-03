/*
 * kss-ftps-entrypoint.h
 *
 *  Created on: 22/07/2011
 *      Author: gonzalo
 */

#ifndef KSS_FTPS_ENTRYPOINT_H_
#define KSS_FTPS_ENTRYPOINT_H_




	void executeOperationRead(ListenSocket , MpsRequest * aReq, RuntimeErrorValidator *);
	void executeOperationWrite(ListenSocket , MpsRequest * aReq, RuntimeErrorValidator *);
	void executeOperationOpen(ListenSocket , MpsRequest * aReq, RuntimeErrorValidator *);
	void executeOperationClose(ListenSocket , MpsRequest * aReq,RuntimeErrorValidator *);
	void executeOperationList(ListenSocket , MpsRequest * aReq ,RuntimeErrorValidator *);




#endif /* KSS_FTPS_ENTRYPOINT_H_ */
