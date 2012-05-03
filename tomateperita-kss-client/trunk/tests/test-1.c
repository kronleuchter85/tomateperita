/*
 * test-1.c
 *
 *  Created on: 20/07/2011
 *      Author: gonzalo
 */

#include <stdlib.h>
#include "linux-commons-socket.h"
#include "kss-client-endpoint.h"

	void testShell(ListenSocket s){
		callKssOperationMount( "VDA1" , s);
		callKssOperationMount( "VDA2" , s);
		callKssOperationMount( "VDA3" , s);
		callKssOperationMount( "VDA4" , s);
		callKssOperationMount( "VDA5" , s);
		callKssOperationMount( "VDA6" , s);

		callKssOperationFormat( "VDA1" , s);
		callKssOperationFormat( "VDA2" , s);
		callKssOperationFormat( "VDA3" , s);
		callKssOperationFormat( "VDA4" , s);
		callKssOperationFormat( "VDA5" , s);
		callKssOperationFormat(  "VDA6" , s);

		callKssOperationList( "/" , s);
		callKssOperationList( "VDA1" , s);
		callKssOperationList( "VDA2" , s);
		callKssOperationList( "VDA3" , s);
		callKssOperationList( "VDA4" , s);
		callKssOperationList( "VDA5" , s);
		callKssOperationList( "VDA6" , s);

		callKssOperationUmount( "VDA1" , s);
		callKssOperationUmount( "VDA2" , s);
		callKssOperationUmount( "VDA3" , s);
		callKssOperationUmount( "VDA4" , s);
		callKssOperationUmount( "VDA5" , s);
		callKssOperationUmount( "VDA6" , s);

		sleep(5);
	}
