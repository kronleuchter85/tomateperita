
/**
 * list-commons.h
 *
 * Define estructuras y funciones
 * para el manejo de listas
 *
 */

#include "memory-commons.h"

#ifndef STRUCT_INDEXED_LIST
#define STRUCT_INDEXED_LIST


	/**
	 * Estructura que define el nodo basico
	 */
	struct LN{
		unsigned int index;
		void * content;
		struct LN * next;
	};
	typedef struct LN ListNode;

	/**
	 * Estructura que define la lista indexada
	 * como un puntero al primer nodo
	 */
	typedef ListNode* SortedList;

#endif


	/**
	 * prototipos de funciones....
	 */

	SortedList createSortedList();
	void deleteSortedList(SortedList * l , HeapHandler (*gethandlerfunction)(void*) 
		,  void (*deletingfunction)(void*) );
	void addElement(SortedList* l , ListNode * n);
	void removeElement(SortedList * list , unsigned int index 
		, HeapHandler hh , void (*deletingfunction)(void*));
	ListNode* createListNode(HeapHandler hh , unsigned int index , void * value);
	void printSortedList(SortedList list , void (*printfunction)(void*));



