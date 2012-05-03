/*
 * fss-mps-services.h
 *
 *  Created on: 10/07/2011
 *      Author: gonzalo
 */


#include "linux-commons-mps.h"
#include "linux-commons-socket.h"
#include "linux-commons-errors.h"

#ifndef FSS_MPS_SERVICES_H_
#define FSS_MPS_SERVICES_H_



	void executeOperation(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator);

	void executeOperationIsExistingFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationGetFileInformation(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationDeleteFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationCreateFile(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationListDirectory(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationFormat(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationAssignAndGetSectors(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator);
	void executeInvalidOperation(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);

	void executeOperationHasVdaFormat(ListenSocket ls , MpsMessage * request, RuntimeErrorValidator * validator);
	void executeOperationUpdateFileSize(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator);

	Bool isOperationExistFile(MpsMessage * req);
	Bool isOperationGetFileInformation(MpsMessage * req);
	Bool isOperationDeleteFile(MpsMessage * req);
	Bool isOperationCreateFile(MpsMessage * req);
	Bool isOperationListDirectory(MpsMessage * req);
	Bool isOperationFormat(MpsMessage * req);
	Bool isOperationAssignAndGetSectors(MpsMessage * req);

	Bool isOperationHasVdaValidFormat(MpsMessage * req);
	Bool isOperationUpdateFileSize(MpsMessage * req);



#endif /* FSS_MPS_SERVICES_H_ */
