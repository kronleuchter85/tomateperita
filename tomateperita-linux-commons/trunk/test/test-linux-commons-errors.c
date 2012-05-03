/*
 * test-linux-commons-errors.c
 *
 *  Created on: 08/08/2011
 *      Author: gonzalo
 */




#include <stdlib.h>


#include "test-linux-commons.h"


	void metodo1(void);
	void metodo2(void);
	void metodo3(void);




	/*
	 * Realiza la configuracion de la suite
	 */
	int linux_commons_errors_configureTestSuite(){

		int suiteInitAndClean(){
			return EXIT_SUCCESS;
		}

		CU_pSuite suite = linux_commons_tests_createSuite(
				"Suite Linux Commons Errors" , suiteInitAndClean , suiteInitAndClean);

		if( linux_commons_tests_addToSuite(suite , "Test Metodo 1" , metodo1) == NULL)
			return EXIT_FAILURE;

		if( linux_commons_tests_addToSuite(suite , "Test Metodo 2" , metodo2) == NULL)
			return EXIT_FAILURE;

		if( linux_commons_tests_addToSuite(suite , "Test Metodo 3" , metodo3) == NULL)
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}



	/*
	 * Metodos que realizan los tests
	 */

	void metodo1(){

		CU_ASSERT_FALSE(1);
	}

	void metodo2(){

	}

	void metodo3(){

	}




