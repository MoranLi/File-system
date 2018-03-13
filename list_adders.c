/*
 * CMPT332 Group 20:
 * Liam Bindle       LRB175  11161519
 * Shahriar Rahman   SHR062  11153915
 */

#include "list.h"

/** list_adders::globals
 * The global variables belonging to list_adders.c are LIST_MEMORY,
 * NODE_MEMORY, and list_memory_initialized.
 */
PREALLOC LIST_MEMORY;
PREALLOC NODE_MEMORY;
int list_memory_initialized = 0;

LIST *ListCreate() {
	LIST* list;

	/* Check if list memory has been initialized yet. */
	if(list_memory_initialized == 0) {
		int i;
		constructPREALLOC(sizeof(LIST), MAX_NUMBER_LISTS, LIST_MEMORY, i);
		constructPREALLOC(sizeof(NODE), MAX_NUMBER_ITEMS, NODE_MEMORY, i);
		list_memory_initialized = 1;
	}

	/* Check if we can allocate another list(pCanAlloc is a macro function). */
	if(pCanAlloc(LIST_MEMORY) == 0) {
		list_errno = LIST_ERROR_NO_MEM;
		return NULL;
	}

	/* Creating the list is valid. Initialize it now. */
	list = (LIST*) pAlloc(LIST_MEMORY);
	list->curr_node = NULL;
	list->first_node = NULL;
	list->last_node = NULL;
	list->num_nodes = 0;
	return list;
}


int ListCount(LIST *list) {
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return -1;
	} else {
		return (int) list->num_nodes;
	}
}


/** _ListEmpty
 * This macro function evaluates to a condition which returns 0 if the list
 * contains items, and 1 if the list is empty.
 */
#define _ListEmpty(list) (list->curr_node == NULL)


/**_InsertFirstNode
 * Since adding the first node to a list is a common bit of code for most of
 * the list adders, we have made it a macro. This macro simply evaluates to
 * a couple statements that inserts a node into a list when the list is
 * empty.
 */
#define _InsertFirstNode(list, node) \
	list->first_node = node; \
	list->last_node = node; \
	list->curr_node = node; \
	node->next_node = NULL; \
	node->prev_node = NULL; \
	++list->num_nodes

/**_InsertBetween
 * All of the list adder functions can be accomplished with an insert
 * between function, where you specify the node before and after the
 * node to be inserted. Since this is a common operation and we want
 * these functions to minimize branching we have made this a macro function.
 *
 * This macro function evaluates to a series of statements that inserts a
 * new node between two other adjascent nodes. This macro also handles cases
 * (1) where the new node is the first node, (2) the new node is the last
 * node, (3) the new node is between two existing node, (4) incrementing
 * the number of nodes in the list.
 */
#define _InsertBetween(list, before, new_node, after) \
	new_node->prev_node = before; \
	new_node->next_node = after; \
	if(before != NULL) { \
		before->next_node = new_node; \
	} else { \
		list->first_node = new_node; \
	} \
	if(after != NULL) { \
		after->prev_node = new_node; \
	} else { \
		list->last_node = new_node; \
	} \
	++list->num_nodes


int ListAdd(LIST *list, void* item) {
	NODE* node;

	/* Check that the given list is not NULL. */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return -1;
	}

	/* Check that we haven't already allocated too many nodes. */
	if(pCanAlloc(NODE_MEMORY) == 0) {
		list_errno = LIST_ERROR_NO_MEM;
		return -1;
	} else {
		node = (NODE*) pAlloc(NODE_MEMORY);
		node->item = NULL;
		node->prev_node = NULL;
		node->next_node = NULL;
	}

	/* Assign the item to the node and insert the node. */
	node->item = item;
	if(_ListEmpty(list)) {
		_InsertFirstNode(list, node);
	} else {
		NODE* before = list->curr_node;
		NODE* after = list->curr_node->next_node;
		_InsertBetween(list, before, node, after);
	}

	/* Don't forget to set the current node to the new node. */
	list->curr_node = node;
	return 0;
}


int ListInsert(LIST *list, void* item) {
	NODE* node;

	/* Check that the given list is not NULL. */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return -1;
	}

	/* Check that we haven't already allocated too many nodes. */
	if(pCanAlloc(NODE_MEMORY) == 0) {
		list_errno = LIST_ERROR_NO_MEM;
		return -1;
	} else {
		node = (NODE*) pAlloc(NODE_MEMORY);
		node->item = NULL;
		node->prev_node = NULL;
		node->next_node = NULL;
	}

	/* Assign the item to the node and insert the node. */
	node->item = item;
	if(_ListEmpty(list)) {
		_InsertFirstNode(list, node);
	} else {
		NODE* before = list->curr_node->prev_node;
		NODE* after = list->curr_node;
		_InsertBetween(list, before, node, after);
	}

	/* Don't forget to set the current node to the new node. */
	list->curr_node = node;
	return 0;
}


int ListAppend(LIST *list, void* item) {
	NODE* node;

	/* Check that the given list is not NULL. */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return -1;
	}

	/* Check that we haven't already allocated too many nodes. */
	if(pCanAlloc(NODE_MEMORY) == 0) {
		list_errno = LIST_ERROR_NO_MEM;
		return -1;
	} else {
		node = (NODE*) pAlloc(NODE_MEMORY);
		node->item = NULL;
		node->prev_node = NULL;
		node->next_node = NULL;
	}

	/* Assign the item to the node and insert the node. */
	node->item = item;
	if(_ListEmpty(list)) {
		_InsertFirstNode(list, node);
	} else {
		NODE* before = list->last_node;
		_InsertBetween(list, before, node, ((NODE*) NULL));
	}

	/* Don't forget to set the current node to the new node. */
	list->curr_node = node;
	return 0;
}


int ListPrepend(LIST *list, void* item) {
	NODE* node;

	/* Check that the given list is not NULL. */
	if(list == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return -1;
	}

	/* Check that we haven't already allocated too many nodes. */
	if(pCanAlloc(NODE_MEMORY) == 0) {
		list_errno = LIST_ERROR_NO_MEM;
		return -1;
	} else {
		node = (NODE*) pAlloc(NODE_MEMORY);
		node->item = NULL;
		node->prev_node = NULL;
		node->next_node = NULL;
	}

	/* Assign the item to the node and insert the node. */
	node->item = item;
	if(_ListEmpty(list)) {
		_InsertFirstNode(list, node);
	} else {
		NODE* after = list->first_node;
		_InsertBetween(list, ((NODE*) NULL), node, after);
	}

	/* Don't forget to set the current node to the new node. */
	list->curr_node = node;
	return 0;
}


void ListConcat(LIST *list1,LIST *list2) {
	 /* Check that neither list is NULL. */
	if(list1 == NULL || list2 == NULL) {
		list_errno = LIST_ERROR_NULL_PTR;
		return;
	}

	/* Check that list1 isn't empty. See special notes. */
	if(_ListEmpty(list1)) {
		list1->first_node = list2->first_node;
		list1->curr_node = list2->curr_node;
		list1->last_node = list2->last_node;
		list1->num_nodes = list2->num_nodes;
	} else if(_ListEmpty(list2) == 0) {
		/* This is the case neither list1 and list2 are empty. */
		list1->last_node->next_node = list2->first_node;
		list2->first_node->prev_node = list1->last_node;
		list1->last_node = list2->last_node;
		list1->num_nodes += list2->num_nodes;
	}

	/* pFree the memory to list2. */
	pFree(LIST_MEMORY, list2);
}
