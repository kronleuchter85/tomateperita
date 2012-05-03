/*
 * kss-vda-component.c
 *
 *  Created on: 12/06/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <string.h>

#include "kss-vda-component.h"
#include "linux-commons.h"

	BOOL vdaEqualityCriteria( VdaComponent * v1, VdaComponent * v2){
		return !strcmp(v1->vdaName , v1->vdaName );
	}
	BOOL vdaSortingCriteria( VdaComponent * v1 , VdaComponent * v2){
		return v1->vdaId < v2->vdaId;
	}
	void vdaListingCriteria(VdaComponent * vda){}

	VdaComponent * buildVdaComponent(int vdaId , char * vdaName , ListenSocket listenSocket){
		VdaComponent * vda = malloc(sizeof(VdaComponent));
		vda->vdaId = vdaId;
		vda->vdaName = vdaName;
		vda->listenSocket = listenSocket;
		return vda;
	}
