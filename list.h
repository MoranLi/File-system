/*
 * CMPT332 Group 20:
 * Liam Bindle       LRB175  11161519
 * Shahriar Rahman   SHR062  11153915
 */
#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

/*                          LIST TYPE DEFINITIONS                            */


/** MAX_NUMBER_ITEMS
 * Defines the maximum number of items (NODES) that can be stored by all
 *instances of LIST's at any given moment.
 */
#define MAX_NUMBER_ITEMS 5000

/** MAX_NUMBER_LISTS
 * Defines the maximum number of lists that can be instantiated at any given
 * point in time.
 */
#define MAX_NUMBER_LISTS 100

/** NODE
 * A NODE is the type that wraps an item in a list. Since the LIST type is a
 * doubly linked list, this type must know about its previous and next nodes.
 */

typedef struct NODE NODE;
struct NODE {
        void *item;
        NODE *next_node;
        NODE *prev_node;
};

/** LIST
 * A LIST is a doubly linked list type. The total number of items stored by
 * all instances of LIST types must NOT exceed MAX_NUMBER_ITEMS at any given
 * time. If this happens, operations trying to add items to list will not
 * be successful.
 */
typedef struct LIST {
  NODE *curr_node;
  NODE *first_node;
  NODE *last_node;
	size_t num_nodes;
} LIST;




/*                         START OF API PROTOTYPES                           */


/** list_memory_initialized
 * An int flag indicating whether or not the list memory has been initialized.
 * This flag will be 0 until the first call the ListCreate is made. At this
 * point the memory for LIST's and NODE's will be allocated and this int will
 * be set to have a value of 1 for the remainder of the programs life.
 */
extern int list_memory_initialized;

/** ListCreate
 * Initializes a new list. Returns pointer to the new list if successful,
 * NULL otherwise.
 */
LIST *ListCreate();

/** ListCount
 * Returns the number of items in the list. Returns -1 on failure.
 */
int ListCount(LIST *list);

/** ListFirst
 * Returns a pointer to the first item in the list. The current position of
 * the list is set to the first item.
 */
void *ListFirst(LIST *list);

/** ListLast
 * Returns a pointer to the last item in the list. The current position of
 * the list is set to the last item.
 */
void *ListLast(LIST *list);

/** ListNext
 * Increments the current position in the list by one. A pointer to the new
 * current item is returned.
 *
 * Note: If this function is called when the current item is the last item
 * in the list, NULL will be return.
 */
void *ListNext(LIST *list);

/** ListPrev
 * Decrements the current position in the list by one. A pointer to the new
 * current item is returned.
 *
 * Note: If this function is called when the current item is the first item
 * in the list, NULL will be return.
 */
void *ListPrev(LIST *list);

/** ListCurr
 * Returns a pointer to the current item in the list. NULL is returned if the
 * list is empty.
 */
void *ListCurr(LIST *list);

/** ListAdd
 * Inserts a new item into the list directly AFTER the current position in the
 * list. The current item in the list is then set to this newly added item.
 *
 * Returns: 0 on success, -1 on failure.
 */
int ListAdd(LIST *list, void* item);

/** ListInsert
 * Inserts a new item into the list directly BEFORE the current position in the
 * list. The current item in the list is then set to this newly added item.
 *
 * Returns: 0 on success, -1 on failure.
 */
int ListInsert(LIST *list, void* item);

/** ListAppend
 * Appends a new item to the END of the list. The current item in the list
 * is then set to this newly added item.
 *
 * Returns: 0 on success, -1 on failure.
 */
int ListAppend(LIST *list, void* item);

/** ListPrepend
 * Inserts a new item at the BEGINNING of the list. The current item in the
 * list is then set to this newly added item.
 *
 * Returns: 0 on success, -1 on failure.
 */
int ListPrepend(LIST *list, void* item);

/** ListRemove
 * Removes the current item from the list. The current position in the list
 * then becomes the next item.
 *
 * Returns: a pointer to the item that was removed. NULL on failure or empty
 * list.
 */
void *ListRemove(LIST *list);

/** ListConcat
 * Appends all the items in list2 to the end of list1. After a successful
 * call to this function list2 no longer exists. The current position in list1
 * is uneffected.
 */
void ListConcat(LIST *list1,LIST *list2);

/** ListFree
 * Deletes the list and free's all the items in the list. Note that itemFree
 * is a pointer to a function that can free an item.
 */
void ListFree(LIST *list, void (*itemFree)(void*));

/** ListTrim
 * Removes the last item in the list and returns a pointer to it. The current
 * item in the list becomes the new last item.
 */
void *ListTrim(LIST *list);

/** ListSearch
 * Searches the list until a comparison returns 1. The comparator function
 * is a function pointer that take two void pointers, the first is an item
 * in the list and the second is comparisonArg. If comparator return 0 then
 * the next item will be tried. If the comparator return 1 the current item
 * is set to this item and a pointer to this item is returned. If comparator
 * returns 0 at the last item NULL will be returned and the current position
 * in the list will be the last item. Note that searching begins with the
 * current item in the list.
 */
void *ListSearch(LIST *list, int(*comparator)(void*, void*), void* comparisonArg);


/*                          END OF API PROTOTYPES                            */





/*                                BONUS MARKS                                */


/** ListError
 * Macro function that will emulate a function returning the most recent error
 * that occured in a function call to the list library. A function prototype
 * of an equivalent function would be the following:
 *
 * const char* ListError();
 *
 * Note: this function works by getting the value of a global int variable
 * called list_errno. A value of 0 means there are no error, otherwise this
 * int stores the value of the most recent error.
 */
#define ListError() LIST_ERROR_MSGS[list_errno];

/**
 * Below an emueration of all the error categories is given.
 */
 #define LIST_NO_ERROR 0
 #define LIST_ERROR_NO_MEM 1
 #define LIST_ERROR_NULL_PTR 2
 #define LIST_ERROR_TRIED_AFTER 3
 #define LIST_ERROR_TRIED_BEFORE 4
 #define LIST_ERROR_UNKNOWN_COMP_RV 5

/**
 * list_errno is a global int which stores the most recent error code.
 * LIST_ERROR_MSGS are messages explaining error codes.
 */
extern int list_errno;
extern const char* LIST_ERROR_MSGS[6];


/*                             END OF BONUS MARKS                            */




/*                            MEMORY MANAGEMENT                              */

/** PREALLOC
 * PREALLOC is a struct that holds information required to manage our LIST
 * and NODE memory. A PREALLOC struct allocates a large block of memory
 * and points to that block of memory with space. A PREALLOC also has a
 * free list of the current free memory addresses within the allocated block
 * (chunks of memory in space). A Top Of Stack (tos) variable indexes the
 * next availible memory address in the free list (called a free stack below
 * because conceptually I see it as more of a stack). This struct also stores
 * the size of the item (item_size, so that memory addresses can be indexed
 * properly). Additionally the max number of items that can be stored in this
 * preallocated chunk are stored in max_items.
 */
typedef struct PREALLOC {
  void* space;
  void** free_stack;
  size_t tos;
  size_t item_size;
  size_t max_items;
} PREALLOC;

/** constructPREALLOC
 * Initializes a PREALLOC structure. This macro function allocates memory and
 * initializes all of the PREALLOC structs members. Note that the free list
 * is initialized (very important).
 */
#define constructPREALLOC(sizeof_item, max_num_items, prealloc, idx) \
  prealloc.space = malloc(sizeof_item * max_num_items); \
  prealloc.free_stack = malloc(max_num_items * sizeof(void*)); \
  for(idx = 0; idx < max_num_items; ++idx) { \
    prealloc.free_stack[idx] = prealloc.space + idx * sizeof_item; \
  } \
  prealloc.tos = max_num_items; \
  prealloc.item_size = sizeof_item; \
  prealloc.max_items = max_num_items

/** freePREALLOC
 * Free's all the memory associated with a PREALLOC struct and uninitializes
 * all of the structs members. For A1-PHASE3 this macro function is never
 * called since we don't ever have to deallocate our list memory, but this
 * macro function is here for completeness.
 */
#define freePREALLOC(prealloc) \
  free(prealloc.space); \
  prealloc.space = NULL; \
  free(prealloc.free_stack); \
  prealloc.free_stack = NULL; \
  prealloc.tos = 0; \
  prealloc.item_size = 0; \
  prealloc.max_items = 0

/** pAlloc
 * This macro function decrements the tos and returns the next availbe memory
 * address in the free list.
 */
#define pAlloc(prealloc) \
  prealloc.free_stack[--prealloc.tos]

/** pFree
 * Adds a memory address to the free list and increments the tos index.
 */
#define pFree(prealloc, mem_ptr) \
  prealloc.free_stack[prealloc.tos++] = mem_ptr

/** pCanAlloc
 * Returns 1 if preallocator can allocate an item, 0 otherwise.
 */
#define pCanAlloc(prealloc) (prealloc.tos != 0)


/** LIST_MEMORY and NODE_MEMORY
 * Global memory chunks for getting memory for LIST's and NODE's.
 */
extern PREALLOC LIST_MEMORY;
extern PREALLOC NODE_MEMORY;


/*                         END OF MEMORY MANAGEMENT                          */





#endif
