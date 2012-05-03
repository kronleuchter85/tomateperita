/*
 * test-vda-2.c
 *
 *  Created on: 19/07/2011
 *      Author: gonzalo
 */


#include "linux-commons-list.h"

#include "kss-vda-service.h"
#include "kss-vda-endpoint.h"


	void testGetChs(){
		Iterator * iteVdas = buildIterator(getConnectedVdas());
		VdaComponent * comp = hasMoreElements(iteVdas)?next(iteVdas) : NULL;
		RuntimeErrorValidator * valid = buildErrorSuccessValidator();

		VdaInformation * vdaInformation = callVdaOperationGetChs(comp , valid);

		printf("cilindros: %i\n" , vdaInformation->tracksCount);
		printf("cabezas: %i\n" , vdaInformation->headCounts);
		printf("sectores: %i\n" , vdaInformation->sectorsCount);

	}
