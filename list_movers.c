/*
 * CMPT332 Group 20:
 * Liam Bindle       LRB175  11161519
 * Shahriar Rahman   SHR062  11153915
 */
#include "list.h"

void *ListFirst(LIST *list){
	 /* Check if list is NULL */
	 if(list == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return NULL;
	 }

	 list->curr_node = list->first_node;

	 /* Handle special case*/
	 if(list->curr_node == NULL) {
		 return NULL;
	 } else {
		 /* Return first item in the list*/
		 return list->curr_node->item;
	 }
}

void *ListLast(LIST *list){
	 /* Check if list is NULL */
	 if(list == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return NULL;
	 }

	 list->curr_node = list->last_node;

	 /* Handle special case */
	 if(list->curr_node == NULL) {
		 return NULL;
	 } else {
		 /* Return first item in the list */
		 return list->curr_node->item;
	 }
}

void *ListNext(LIST *list){
	/* Check if list is NULL */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return NULL;
	}

	/* Check if the current node is also the last node */
	/* Note: this condition will also catch if the list is empty */
	if(list->curr_node == list->last_node) {
		/* Note that this is NOT an error */
		return NULL;
	}

	/* Move to the next node and return its item */
	list->curr_node = list->curr_node->next_node;
	return list->curr_node->item;
}

void *ListPrev(LIST *list){
	/* Check if list is NULL */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return NULL;
	}

	/* Check if the current node is also the first node */
	/* Note: this condition will also catch if the list is empty */
	if(list->curr_node == list->first_node) {
		/* Note that this is NOT an error */
		return NULL;
	}

	/* Move to the previous node and return its item */
	list->curr_node = list->curr_node->prev_node;
	return list->curr_node->item;
}


void *ListCurr(LIST *list){
	/* Check if list is NULL */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return NULL;
	}

	/* Check that the current node isn't NULL (ie. the list is empty) */
	if(list->curr_node == NULL) {
		/* Note that this is NOT an error. */
		return NULL;
	}

	/* Return the current item */
	return list->curr_node->item;
}
void *ListSearch(LIST *list,
							   int(*comparator)(void*, void*),
								 void* comparisonArg) {
	 NODE* node;
	 int compare_value = 0;

	 /* Check if either of the first two arguments are NULL */
	 if(list == NULL || comparator == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return NULL;
	 }

	 /* Loop through the nodes until we get a match or we get to NULL */
	 node = list->curr_node;
	 while(node != NULL &&  compare_value == 0) {
		 compare_value = comparator(node->item, comparisonArg);
		 if(compare_value == 0) node = node->next_node;
	 }

	 if(node != NULL) { /* We found a match */
		 /* Check that the return value was 1. If it wasn't then set list_errno */
		 if(compare_value != 1) {
			 list_errno = LIST_ERROR_UNKNOWN_COMP_RV;
		 }
		 /* Set current node and return the item */
		 list->curr_node = node;
		 return list->curr_node->item;
	 } else { /* No match was found */
		 /* Set the current node to the end of the list and return NULL */
		 list->curr_node = list->last_node;
		 return NULL;
	 }
}
