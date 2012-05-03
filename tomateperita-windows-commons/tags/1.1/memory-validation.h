/**
 * memory-validation.h
 *
 * Define funciones comunes para la validacion
 * referente al manejo de memoria
 *
 */

#include "headers-commons.h"
#include "validation.h"
#include "commons.h"

	/**
	 * Validaciones del heap........
	 */
	int validateHeapCreation(HeapHandler h);
	int validateHeapDestruction(int code);
	int validateMemoryAllocation(void * ptr);
	int validateMemoryDeletion(int code);


