#include "LL.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// PRE:
// POST: Creates and returns a new, empty list
LL_t* LLcreate()
{
	LL_t* result = malloc(sizeof(LL_t));
	assert(result);
	result->head = NULL;
	result->tail = NULL;
	return result;
}

// PRE:
// PARAM: list = list
// POST: Returns 1 if list is empty, 0 otherwise
int LLis_empty(LL_t* list)
{
	return list->head == NULL;
}

// PRE:
// PARAM: list = list to be appended to
//        value = value to be appended
// POST: Adds a new element to the tail of list
void LLappend(LL_t* list, int value)
{
	// Build the node to be appended
	node_t* new_node = malloc(sizeof(node_t));
	assert(new_node);
	new_node->data = value;
	new_node->next = NULL;
	
	// List is empty
	if(list->tail == NULL){
		assert(list->head == NULL);
		list->head = list->tail = new_node;
	}else{
		// List not empty
		list->tail->next = new_node;
		list->tail = new_node;
	}		
}

// PRE:
// PARAM: list = list to be inserted into
//        value = value to be inserted
// POST: Adds a new element to the head of list
void LLinsert_head(LL_t* list, int value)
{
	// Build the node to be inserted
	node_t* new_node = malloc(sizeof(node_t));
	assert(new_node);
	// Set values of node
	new_node->data = value;
	new_node->next = list->head; //points to read of list	
	// Make new head
	list->head = new_node;
	// Make tail if list was empty
	if(list->tail == NULL){
		list->tail = list->head;
	}	
}

// PRE: list is non-empty
// PARAM: list = list to remove value from
// POST: Removes element from head of list, and returns value
int LLremove_head(LL_t* list)
{
	assert(list->head != NULL);	
	node_t* temp = list->head;
	int result = temp->data;
	list->head = list->head->next; //set new head
	// Set tail to NULL if list is empty
	if(list->head == NULL){
		list->tail = NULL;
	}
	free(temp);
	return result;	
}

// PRE:
// PARAM: list = list to be printed
// POST: prints list elements from head to tail
void LLprint(LL_t* list)
{
	for(node_t* current = list->head;
		current != NULL; 
		current = current->next){

		printf("%d ", current->data);
	}
	printf("\n");
	/*	
	node_t* current = list->head;	
	// Traverse the list	
	while(current != NULL){
		printf("%d ", current->data);
		current = current->next;
	}
	*/
}

// PRE:
// PARAM: list = list to be destroyed
// POST: frees all memory associated with list
void LLdestroy(LL_t* list)
{
	node_t* current = list->head;
	while(list->head != NULL){
		list->head = list->head->next;
		free(current);
		current = list->head;
	}
	free(list);
	list = NULL;
}

// PRE:
// PARAM: list1 = "left hand list"
//        list2 = "right hand list"
// POST: concatenates list1 and list 2 then frees memory
//       associated with list2
void LLconcatenate(LL_t* list1, LL_t* list2)
{
	// list1 is empty
	if(list1->head == NULL){
		list1->head = list2->head;
	}else{
		list1->tail->next = list2->head;
	}
	// list2 is not empty
	if(list2->tail != NULL){
		list1->tail = list2->tail;
	}
	// Clean up list2
	free(list2);
	list2 = NULL;
}

// PRE:
// PARAM: list = list to be searched
//        target = value to be searched for
// POST: returns 1 if target is in list, 0 otherwise
int LLsearch(LL_t* ls, int target)
{
	return LLsearch_helper(ls->head, target);
}

// PRE:
// PARAM: list = list to be searched
//        target = value to be searched for
// POST: recursive helper method for LLsearch
int LLsearch_helper(node_t* current, int target)
{
	if(current == NULL){
		return 0;
	}else if(current->data == target){
		return 1;
	}else{	
		return LLsearch_helper(current->next, target);
	}
}

// PRE:
// PARAM: list = list that value is to be removed from
//        target = value to be removed
// POST: removes first incidence of target from list,
//       returns 1 if target removed, 0 otherwise
int LLremove(LL_t* list, int target)
{
	node_t* current = list->head;
	if(current == NULL){
		return 1;
	} 
	// Look at head of the list
	if(current->data == target){
		list->head = current->next;
		// If list is now empty
		if(list->head == NULL){
			list->tail = NULL;
		}
		free(current);
		return 1;
	}
	// Value in rest of list
	while(current->next != NULL){ //look ahead
		if(current->next->data == target){
			node_t* temp = current->next; //node to remove
			// Set tail if node to remove is tail
			if(temp == list->tail){
				list->tail = current;
			}
			current->next = current->next->next;
			free(temp);
			return 1;
		}
		current = current->next; //move to next node
	}
	return 0;
}
