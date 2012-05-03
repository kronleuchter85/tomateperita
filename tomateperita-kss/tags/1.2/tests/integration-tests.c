/*
 * integration-test-fss.c
 *
 *  Created on: 16/07/2011
 *      Author: gonzalo
 */

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
