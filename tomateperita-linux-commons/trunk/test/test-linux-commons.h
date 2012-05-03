/*
 * test-linux-commons.h
 *
 *  Created on: 08/08/2011
 *      Author: gonzalo
 */

#include <CUnit/Basic.h>
#include <CUnit/Automated.h>


#ifndef TEST_LINUX_COMMONS_H_
#define TEST_LINUX_COMMONS_H_


	int linux_commons_tests_configure();

	int linux_commons_errors_configureTestSuite();

	CU_pSuite linux_commons_tests_createSuite(
			char * suiteName ,
			CU_InitializeFunc pInit,
	        CU_CleanupFunc pClean);

	CU_pTest linux_commons_tests_addToSuite(
			CU_pSuite pSuite,
			const char* testName,
			CU_TestFunc pTestFunc);






	void linux_commons_errors_runTestSuite();


#endif /* TEST_LINUX_COMMONS_H_ */
