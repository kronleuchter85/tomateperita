/**
 * memory-commons.h
 *
 * Funciones comunes del manejo de memoria...
 *
 */


#include "windows-commons-memory.h"


	/**
	 * Crea un HEAP con el tamaño solicitado
	 * devolviendo el handler correspondiente.
	 */
	HeapHandler createHeap(int heapSize){
		return HeapCreate(0,heapSize,0);
	}

	/**
	 * Elimina el Heap manejado por el HeapHandler
	 */
	int destroyHeap(HeapHandler h){
		return HeapDestroy(h);
	}

	void * allocateMemory(HeapHandler h , int size){
		return HeapAlloc(h , HEAP_ZERO_MEMORY , size);
	}

	void freeMemory(HeapHandler h , void * ptr){
		HeapFree(h , 0 , ptr);
	}

