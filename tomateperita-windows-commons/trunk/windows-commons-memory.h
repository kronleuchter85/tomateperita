/**
 * memory-commons.h
 *
 * Define tipos y funciones comunes referentes
 * al manejo de memoria.
 *
 */


#include "windows-commons-headers.h"


	/**
	 * Alias para el HeapHandler HANDLE
	 */
	typedef HANDLE HeapHandler;


	/**
	 * Funciones utilizadas para el manejo de memoria
	 */
	HeapHandler createHeap(int heapSize);
	int destroyHeap(HeapHandler h);
	void * allocateMemory(HeapHandler h , int size);
	void freeMemory(HeapHandler h , void * ptr);