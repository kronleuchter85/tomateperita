/*
 * fss-mps-services.h
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */


#include "linux-commons-mps.h"
#include "linux-commons-socket.h"
#include "linux-errors.h"

#ifndef FSS_MPS_SERVICES_H_
#define FSS_MPS_SERVICES_H_



	void executeOperation(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator);

	void executeOperationIsExistingFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationGetFileInformation(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationDeleteFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationCreateFile(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationListDirectory(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationFormat(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);
	void executeOperationAssignAndGetSectors(ListenSocket ls , MpsRequest * request , RuntimeErrorValidator * validator);
	void executeInvalidOperation(ListenSocket ls , MpsRequest * request, RuntimeErrorValidator * validator);

	Bool isOperationExistFile(MpsRequest * req);
	Bool isOperationGetFileInformation(MpsRequest * req);
	Bool isOperationDeleteFile(MpsRequest * req);
	Bool isOperationCreateFile(MpsRequest * req);
	Bool isOperationListDirectory(MpsRequest * req);
	Bool isOperationFormat(MpsRequest * req);
	Bool isOperationAssignAndGetSectors(MpsRequest * req);



#endif /* FSS_MPS_SERVICES_H_ */
