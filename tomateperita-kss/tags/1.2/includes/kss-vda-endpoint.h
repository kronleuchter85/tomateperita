/*
 * kss-vda-endpoint.h
 *
 *  Created on: 18/07/2011
 *      Author: gonzalo
 */

#include "linux-errors.h"
#include "linux-commons-list.h"

#include "kss-vda-state.h"

#ifndef KSS_VDA_ENDPOINT_H_
#define KSS_VDA_ENDPOINT_H_

	List callVdaOperationGetSectores(VdaComponent * vdaComp ,
			int sector1 , int sector2 ,
			RuntimeErrorValidator * validator);

	void callVdaOperationPutSectores(VdaComponent * vdaComp ,
			int sector1 , char * content1 , int sector2 , char * content2 ,
			RuntimeErrorValidator * validator);

	VdaInformation * callVdaOperationGetChs(VdaComponent * vdaComp ,
			RuntimeErrorValidator * validator);

#endif /* KSS_VDA_ENDPOINT_H_ */
