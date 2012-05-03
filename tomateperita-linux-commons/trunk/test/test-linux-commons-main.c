/*
 * test-linux-commons-errors.c
 *
 *  Created on: 08/08/2011
 *      Author: gonzalo
 */




#include <stdlib.h>


#include "test-linux-commons.h"


	CU_ErrorCode linux_commons_tests_initialize(){
		return CU_initialize_registry();
	}


	void linux_commons_tests_finalize(){
		CU_cleanup_registry();
	}


	CU_pSuite linux_commons_tests_createSuite(char * suiteName , CU_InitializeFunc pInit,
            CU_CleanupFunc pClean){

		CU_pSuite pSuite = CU_add_suite(suiteName, pInit, pClean);
		if (NULL == pSuite) {
			CU_cleanup_registry();
			return NULL;
		}

		return pSuite;

	}


	CU_pTest linux_commons_tests_addToSuite(CU_pSuite pSuite, const char* testName,
			CU_TestFunc pTestFunc){

		CU_pTest test = CU_add_test(pSuite, testName, pTestFunc);
		if(test == NULL){
			linux_commons_tests_finalize();
			return NULL;
		}

		return test;
	}




	void linux_commons_tests_run(){
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();

		//NOTA:
		//Si se quiere crear un archivo con las estadisticas hay
		//que descomentar estas lineas y comentar las 2 anteriores
		//
		CU_set_output_filename( "test-tomateperita-commons" );
		CU_list_tests_to_file();
		CU_automated_run_tests();
	}




	int main(){

		CU_pSuite pSuite = NULL;

		if (CUE_SUCCESS != linux_commons_tests_initialize())
			return CU_get_error();

		if(linux_commons_tests_configure() == EXIT_FAILURE){
			linux_commons_tests_finalize();
			return CU_get_error();
		}

		linux_commons_tests_run();

		linux_commons_tests_finalize();

		return CU_get_error();

	}
