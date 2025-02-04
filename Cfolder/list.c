/**************/
/*   list.c   */
/**************/


#include <stdio.h>
#include <stdlib.h>
#include "../Hfolder/list.h"
#include "../Hfolder/Customer.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if ( pList == NULL ) 	
		return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;
    
	if ( !pNode )
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if ( tmp != NULL )  {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}

	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode)
{
    NODE* tmp;
    if (!pNode)
        return False;
    tmp = pNode->next;
    if(!tmp)
        return False;
    
    pNode->next = tmp->next;
    
    if(tmp != NULL)
    {
        free(tmp);
    }
    return True;
}

//////////////////////////////////////////////////////////////
// Delete Node with Optional Key Release
// Aim:     Delete a node from the list and optionally free its key
// Input:   - pNode: Pointer to the node BEFORE the one to be deleted
//          - freeKey: Boolean flag indicating whether to free the key
// Output:  TRUE if the operation succeeded, FALSE otherwise
//////////////////////////////////////////////////////////////
BOOL L_delete_extended(NODE* pNode, BOOL freeKey)
{
    NODE* tmp;

    if (!pNode)
        return False;
    tmp = pNode->next;
    
    if (!tmp)
        return False;

    // Update the link to skip the deleted node
    pNode->next = tmp->next;

    // Free the key if required
    if (freeKey && tmp->key) {
        free(tmp->key);
    }

    free(tmp);

    return True;
}



/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE* L_find(const NODE* pNode, DATA Value)
{
	const NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if (tmp->key == Value)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList)
{
	NODE *tmp;
	BOOL cont = True;
	if ( !pList ) 
		return False;

	tmp = &(pList->head);
	while (cont)
		cont = L_delete(tmp);
		
	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList)
{
	NODE	*tmp;
	int		c = 0;

	if ( !pList ) 
		return False;

	printf("\n");
	tmp = pList->head.next;
	while(tmp != NULL)
	{
		printf(" %d ---> ", tmp->key);
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}
