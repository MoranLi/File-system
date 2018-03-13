/*
 * CMPT332 Group 20:
 * Liam Bindle       LRB175  11161519
 * Shahriar Rahman   SHR062  11153915
 */
#include "list.h"

void ListFree(LIST *list, void(*itemFree)(void*)){
	 NODE* node;

	 /* Check that neither arguments are NULL */
	 if(list == NULL || itemFree == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return;
	 }

	 /* Call itemFree with all the items in the list. Also pFree all nodes. */
	 node = list->first_node;
	 while(node != NULL) {
		 itemFree(node->item);
		 pFree(NODE_MEMORY, node);
		 node = node->next_node;
	 }

	 /* Don't forget to pFree the list */
	 pFree(LIST_MEMORY, list);
}

/** _ListRemoveNode
 * Removing a node from a list is done in both ListTrim and ListRemove. There
 * are a number of special cases which must be handled with care. A short
 * discussion of these follows:
 *
 * (1) Removing the first node
 * (2) Removing the last node
 * (3) Removing the only node
 * (4) Removing a node between two other nodes
 *
 * This macro function ensures that both ListTrim and ListRemove use the same
 * code an making their implementation conceptually similar. Note that this
 * macro handles all fixing pointer references, list num_nodes.
 *
 * Important: This macro does NOT call pFree on the node to be removed!
 *
 * Special note: this macro only modifies list->curr_node in the event all
 * nodes have been removed.
 */
#define _ListRemoveNode(list, node) \
	if(node->prev_node == NULL) { \
		/* Removing first node */ \
		list->first_node = node->next_node; \
		if(list->first_node != NULL) { \
			/* There is atleast one node still */ \
			list->first_node->prev_node = NULL; \
		} \
	} else { \
		/* There is a node before this one */ \
		node->prev_node->next_node = node->next_node; \
	} \
	if(node->next_node == NULL) { \
		/* Removing the last node*/ \
		list->last_node = node->prev_node; \
		if(list->last_node != NULL) { \
			/* There is still atleast 1 node in the list */ \
			list->last_node->next_node = NULL; \
		} \
	} else { \
		/* There is a node before this one */ \
		node->next_node->prev_node = node->prev_node; \
	} \
	--list->num_nodes; \
	if (list->num_nodes == 0) { \
		list->curr_node = NULL; \
	}


void *ListTrim(LIST *list){
	 void* item;
	 NODE* last_node;

	 /* Check that list is not NULL */
	 if (list == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return NULL;
	 }

	 last_node = list->last_node;

	 /* If the list is empty simply return NULL */
	 if (last_node == NULL) {
		 return NULL;
	 } else {
		 /* Using _ListRemoveNode is okay since there's atleast one node */
		 item = last_node->item;
		 _ListRemoveNode(list, last_node);
		 pFree(NODE_MEMORY, last_node);
		 /* Set the new current node */
		 list->curr_node = list->last_node;
		 return item;
	 }
}


void *ListRemove(LIST *list){
	 void* item;
	 NODE* node_to_remove;

	 /* Check that list is not NULL */
	 if(list == NULL) {
		 list_errno = LIST_ERROR_NULL_PTR;
		 return NULL;
	 }

	 node_to_remove = list->curr_node;
	 if(node_to_remove == NULL) { /* Check if the list is empty*/
		 /* Note that this is NOT an error */
		 return NULL;
	 } else {
		 /* Using _ListRemoveNode is okay since there's atleast one node */
		 item = node_to_remove->item;
		 _ListRemoveNode(list, node_to_remove);
		 pFree(NODE_MEMORY, node_to_remove);
		 /* Set the new current node */
		 list->curr_node = node_to_remove->next_node;

		 if(list->curr_node == NULL) {
			 /* catch case where removed last node */
			list->curr_node = list->last_node;
		 }
		 return item;
	 }
}
