#ifndef _LL_T_
#define _LL_T_

#include "LL-node.h"

typedef struct {
	node_t* head;
	node_t* tail;
} LL_t;

// PRE:
// POST: Creates and returns a new, empty list
LL_t* LLcreate();

// PRE:
// PARAM: list = list
// POST: Returns 1 if list is empty, 0 otherwise
int LLis_empty(LL_t* list);

// PRE:
// PARAM: list = list to be appended to
//        value = value to be appended
// POST: Adds a new element to the tail of list
void LLappend(LL_t* list, int value);

// PRE:
// PARAM: list = list to be inserted into
//        value = value to be inserted
// POST: Adds a new element to the head of list
void LLinsert_head(LL_t* list, int value);

// PRE: list is non-empty
// PARAM: list = list to remove value from
// POST: Removes element from head of list, and returns value
int LLremove_head(LL_t* list);

// PRE:
// PARAM: list = list to be printed
// POST: prints list elements from head to tail
void LLprint(LL_t* list);

// PRE:
// PARAM: list = list to be destroyed
// POST: frees all memory associated with list
void LLdestroy(LL_t* list);

// PRE:
// PARAM: list1 = "left hand list"
//        list2 = "right hand list"
// POST: concatenates list1 and list 2 then frees memory
//       associated with list2
void LLconcatenate(LL_t* list1, LL_t* list2);

// PRE:
// PARAM: list = list to be searched
//        target = value to be searched for
// POST: returns 1 if target is in list, 0 otherwise
int LLsearch(LL_t* ls, int target);

// PRE:
// PARAM: list = list to be searched
//        target = value to be searched for
// POST: recursive helper method for LLsearch
int LLsearch_helper(node_t* current, int target);

// PRE:
// PARAM: list = list that value is to be removed from
//        target = value to be removed
// POST: removes first incidence of target from list,
//       returns 1 if target removed, 0 otherwise
int LLremove(LL_t* list, int target);

#endif


