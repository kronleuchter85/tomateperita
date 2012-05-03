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

		List chs = callVdaOperationGetChs(comp , valid);
		Iterator * iteChs = buildIterator(chs);

		char * cilinders = hasMoreElements(iteChs)?next(iteChs) : NULL;
		char * heads = hasMoreElements(iteChs)?next(iteChs) : NULL;
		char * sectors = hasMoreElements(iteChs)?next(iteChs) : NULL;

		info(concatAll(2 , "Cilindros: " , cilinders));
		info(concatAll(2, "Cabezas: " , heads));
		info(concatAll(2, "Sectores: " , sectors));
	}
