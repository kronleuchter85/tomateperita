#include <stdio.h>
#include <string.h>
#include "commons.h"

void testRandomKeys (void){
		
	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));
	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));

	printf("%s\n", generateRandomKey(20 , getGlobalHeap()));


}