/*
 * tests4.c
 *
 *  Created on: 24/07/2011
 *      Author: gonzalo
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "linux-commons.h"
#include "linux-commons-logging.h"
#include "linux-commons-file.h"
#include "linux-commons-errors.h"
#include "linux-commons-list.h"

#include "fss-configuration.h"
#include "fss-state.h"
#include "fss-utils.h"
#include "fss-fs-management.h"




	void testSerializationList(){

		printf("******************************************************************************\n");
		List files = getFileSectors("/home/gonzalo/tomateperita-data/VDA1/archivo1.txt");
		Iterator * filesIte = buildIterator(files);
		while(hasMoreElements(filesIte)){
			printf("%s\n" , next(filesIte));
		}

		RuntimeErrorValidator * v = buildErrorSuccessValidator();
		FileInformation * infor = getFileInformation("VDA1" , "archivo1.txt" , v);

		Iterator * ite = buildIterator(infor->sectors);

		while(hasMoreElements(ite)){
			printf("%s\n" , (char*)next(ite));
		}

		printf("***************************************************\n" );
		printf("%s\n" , serializeList(infor->sectors , ","));

	}
