#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "windows-commons-list.h"
#include "windows-commons-logging.h"
#include "windows-commons-mps.h"
#include "windows-commons-socket.h"
#include "windows-commons-mps-encoder.h"





	char * getMpsOperationName(char * payload, RuntimeErrorValidator * v , HeapHandler hh){
		return subString(payload , 0 , MPS_FIELD_LENGTH_OPERATION_NAME -1 , hh);
	}

	List getMpsArguments(char * payload , int argsCount, RuntimeErrorValidator * v , HeapHandler hh){

		char * searchingString = payload + MPS_FIELD_LENGTH_STANDARD_PAYLOAD + MPS_FIELD_LENGTH_OPERATION_SEPARATOR;

		if(argsCount > 0){
			List args = buildSringsList(hh);

			if(argsCount == 1){
				char * value = decode(strdup(searchingString) );
				addNode(args , value , hh);

			} else{
				List tokenized = tokenize(searchingString , ' ' , hh);
				Iterator * ite = buildIterator(tokenized, hh);
				while(hasMoreElements(ite)){
					char * arg = decode(next(ite));
					addNode(args , arg, hh);
				}
			}
			return args;
		}
		return NULL;
	}


	char * getMpsStatusCode(char * header, RuntimeErrorValidator * v , HeapHandler hh){
		return subString(header + MPS_FIELD_LENGTH_DESCRIPTOR_ID ,0 , MPS_FIELD_LENGTH_PAYLOAD_DESCRIPTOR-1 , hh);
	}

	int getMpsArgsCount(char * payload, RuntimeErrorValidator * v , HeapHandler hh){
		char * count = subString(payload + MPS_FIELD_LENTTH_STANDARD_PAYLOAD_FIRST_PART , 0 , MPS_FIELD_LENGTH_ARGUMENTS_COUNT-1 , hh);
		return atoi(count);
	}

	

	void replyValidationError(ListenSocket ls , MpsMessage * request , RuntimeErrorValidator * validator , HeapHandler hh){
		MpsMessage * resp = buildMpsMessage(request->descriptorId ,
				MPS_RESPONSE_STATUS_CODE_ERROR,
				request->operationName ,
				buildStringsListWithValues(hh, 1, validator->errorDescription),  hh);
		sendMpsMessage(resp , ls , validator , hh);
	}

	

	int calculatePayloadLength(char * serializedValues , HeapHandler hh){
		int payloadLength = MPS_FIELD_LENGTH_STANDARD_PAYLOAD ;
		if( !equalsStrings(trim(serializedValues , hh) , "")){
			payloadLength += MPS_FIELD_LENGTH_OPERATION_SEPARATOR + (int)strlen(serializedValues);
		}

		return payloadLength;
	}



