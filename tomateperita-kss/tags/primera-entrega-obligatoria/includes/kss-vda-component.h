/*
 * kss-vda-component.h
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */


#include "linux-commons-socket.h"

#ifndef KSS_VDA_COMPONENT_H_
#define KSS_VDA_COMPONENT_H_

	typedef struct {
		int vdaId;
		char * vdaName;
		ListenSocket listenSocket;
	}VdaComponent;


	BOOL vdaEqualityCriteria( VdaComponent * , VdaComponent *);
	BOOL vdaSortingCriteria( VdaComponent * , VdaComponent *);
	void vdaListingCriteria(VdaComponent *);
	VdaComponent * buildVdaComponent(int vdaId , char * vdaName , ListenSocket listenSocket);

#endif /* KSS_VDA_COMPONENT_H_ */
