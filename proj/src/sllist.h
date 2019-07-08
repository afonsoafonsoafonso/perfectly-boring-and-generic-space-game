#pragma once

/** @defgroup sllist sllist
 * @{
 * 
 * 
 * 
 *  Functions and structs for a singly-linked generic list. Code was made by user 'oddlogic' and we got it from a post from him in Stack Exchange (https://codereview.stackexchange.com/questions/26732/singly-linked-list-library). We then compared it to other open linked-list APIs from the internet and we were satisfied with this particular library so we went ahead with it and used it. We tried to message him to really check if he wouldn't mind us using his API but he hasn't been online since... 2013. Anyways, any code displayed on Stack Exchange is open for use, or that's what the conclusion we get to after some search on the internet.
 */

/**
 * @brief Represents one node, an element, of the linked list. Has a pointer to it's data and a pointer to the next element on the list
 * 
 */
struct lnode {
        void *data; //!< Node data
        struct lnode *next; //!< Pointer to next node
};
/**
 * The list structure.
 *
 * Metadata is contained here. A pointer to the first and last nodes in the list
 * allows for several operations to be performed more quickly. There is also
 * another pointer-to-node member variable for storing the location of a
 * "current" or active node that presumably will have operations performed on
 * it. Finally there is a size variable containing the total number of nodes.
 * Note that the first index of the list is considered index zero.
 */


/**
 * @brief Linked list structure. Pointers to the list head, tail and a pointer to a member storing a "current" member which can be used for operations and stuff. Also has a integer with the total size of the linked list
 * 
 */
struct sllist {
        struct lnode *head; //!< Pointer to linked list head
        struct lnode *tail; //!< Pointer to linked list tail
        struct lnode *current; //!< Pointer to a "current" node for user operations
        int size; //!< Linked list number of members
};
/**
 * Create a new list.
 *
 * Returns a pointer to a new, empty list. If allocation fails, returns NULL.
 */

/**
 * @brief Creates a new list.
 * 
 * @return struct sllist*  Returns a pointer to a new and empty lists. If non-successful returns NULL
 */
struct sllist* sllist_create(void);

/**
 * @brief Destroys a list
 * 
 * @param sllist Frees the memory of the list and all it's nodes
 */
void sllist_destroy(struct sllist *sllist);

/**
 * @brief Adds a node to the front of the list
 * 
 * @param sllist List where node will be added
 * @param data Data of the node that will be added to the list
 * @return int 0 uppon success, -1 otherwise
 */
int sllist_push_front(struct sllist *sllist, void *data);

/**
 * Append node to a list.
 *
 * Adds a node to the end of the list. If allocation fails, returns -1,
 * otherwise returns 0.
 */

/**
 * @brief Adds a node to the back of the list
 * 
 * @param sllist List where node will be added
 * @param data Data of the added node
 * @return int 0 uppon success, -1 otherwise
 */
int sllist_push_back(struct sllist *sllist, void *data);

/**
 * @brief Removes node from the front (the first node)
 * 
 * @param sllist List from which the first node will be removed
 * @return void* Returns a pointer to the removed node so that it can be used even after being removed
 */
void* sllist_pop_front(struct sllist *sllist);

/**
 * @brief Removes node from the back (last node)
 * 
 * @param sllist List from which the last node will be removed
 * @return void* Returns a pointer to the removed node so that it can be used even after being removed
 */
 void* sllist_pop_back(struct sllist *sllist);

/**
 * Extract a node after the node at the specified index.
 *
 * Remove the specified node from the linked list, save a pointer to the data,
 * free the node (but do not free the data itself), and return a pointer to the
 * data so that it can be used. If the list is empty or the node doesn't exist
 * in the list, returns NULL. Attempting to extract after the tail will also
 * return NULL.
 */

/**
 * @brief Removes a node after the specified index
 * 
 * @param sllist List from which the node will be removed
 * @param index Index of the previous node in referece to the one that will be removed
 * @return void* Returns a pointer to the removed node so that it can be used even after being removed
 */
void* sllist_extract_after(struct sllist *sllist, int index);
