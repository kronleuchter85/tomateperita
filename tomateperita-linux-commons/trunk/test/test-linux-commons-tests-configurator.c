/*
 * test-linux-commons-tests-launcher.c
 *
 *  Created on: 08/08/2011
 *      Author: gonzalo
 */

#include "test-linux-commons.h"



	int linux_commons_tests_configure(){


		int result = 1;

		result &= linux_commons_errors_configureTestSuite();



		return result;

	}
