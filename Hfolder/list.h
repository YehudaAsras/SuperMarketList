/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_

#include "def.h"


/*** Definitions ***/

// List
typedef struct 
{
	NODE head;
}LIST;


/*** Function prototypes ***/

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, DATA Value);	// add new node after *pNode

BOOL L_delete(NODE* pNode);					// erase node after *pNode

BOOL L_delete_extended(NODE* pNode, BOOL freeKey); //erase node after *pNode with Optional Key Release

const NODE* L_find(const NODE* pNode, DATA Value);		// return a pointer to the node 

BOOL L_free(LIST* pList);					// free list memory

int L_print(const LIST* pList);					// print the list content

#endif
