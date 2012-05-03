/*
 * integration-test-fss.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */
#include <stdlib.h>
#include <stdio.h>

#include "linux-commons.h"
#include "linux-commons-mps.h"
#include "linux-commons-logging.h"
#include "linux-commons-errors.h"

#include "kss-core-services.h"
#include "kss-utils.h"
#include "kss-shell-service.h"
#include "kss-state.h"


	void integrateFss(){
		testFormat();
		testIsExistingFile();
		testGetFileInformation();
		testCreateFile1();
		testCreateFile2();
		testCreateFile3();
		testCreateFile4();
		testGetFileInformationCreated();
		testDeleteFile();
		testGetFileInformationDeleted();
		testAssignAndGetSectors();
		testListDirectory();
		testListInexistingDirectory();
	}


	void integrateVda(){
		testGetChs();
		testCommunication();
		testCommunication();
		testGetChs();
	}


	void formatAndMountVda(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		format("VDA1" , validator);
		mountVda("VDA1" , validator);
	}


	void unmountVda(){
		RuntimeErrorValidator * validator = buildErrorSuccessValidator();
		umountVda("VDA1" , validator);
	}

	void integrateFtps(){

		formatAndMountVda();

		testOpenFileForWrite();
		testOpenFileForRead();


		unmountVda();
	}
