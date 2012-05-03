/**
 * memory-validation.c
 *
 * Funciones comunes de validacion de memoria.
 *
 */


#include "memory-validation.h"


	int validateHeapCreation(HeapHandler h){
		if( h == NULL){
			printf("no se pudo crear el Heap!!!\n");		
			printf("%s\n" , getErrorMessage());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateHeapDestruction(int code){
		if(code == 0){
			printf("no se pudo destruir el Heap!!!\n");
			printf("%s\n" , getErrorMessage());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateMemoryAllocation(void * ptr){
		if( ptr == NULL){
			printf("no se pudo reservar espacio de memoria en el Heap!!!\n");		
			printf("%s\n" , getErrorMessage());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

	int validateMemoryDeletion(int code){
		if(code == 0){
			printf("no se pudo liberar la memoria del Heap!!!\n");
			printf("%s\n" , getErrorMessage());
			return RESULT_WITH_ERRORS;
		}
		return RESULT_SUCCESSFULLY;
	}

