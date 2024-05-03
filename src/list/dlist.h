/**
 * \file dlist.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-22
 * \brief Doubly Linked List
 */
#ifndef __DLIST__H__
#define __DLIST__H__
#include <stdlib.h>
#include <string.h>
#include "../impl/iter.h"
/**
 * \brief A Doubly Linked List node. Represents a location in a `dlist_t`.
 */
typedef struct dlnode_t {
	void *data;
	struct dlnode_t *next;
	struct dlnode_t *prev;
} dlnode_t;

/**
 * \brief A generic doubly linked list. `dlist_t` is an Iterable object.
 */
typedef struct {
	dlnode_t *head;
	dlnode_t *tail;
	size_t size;
	size_t data_size;
} dlist_t;

/**
 * \brief Add `data` to the start of the `list`.
 *
 * \param[in] list The list to add to
 * \param[in] data The data to add
 */
void dll_addh(dlist_t *list, void *data);

/**
 * \brief Add `data` to the end of the `list`.
 *
 * \param[in] list The list to add to
 * \param[in] data The data to add
 */
void dll_addt(dlist_t *list, void *data);

/**
 * \brief Add `data` after a `node` from `list`.
 *
 * \param[in] list The list to add to
 * \param[in] node The node to add after
 * \param[in] data The data to add
 */
void dll_add_after(dlist_t *list, dlnode_t *node, void *data);

/**
 * \brief Add `data` at position `index` in `list`.
 *
 * \param[in] list The list to add to
 * \param[in] index The index of where to add the data to
 * \param[in] data The data to add
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
void dll_add_index(dlist_t *list, size_t index, void *data);

/**
 * \brief Removes the first element from a list. If there are no elements in the
 * list then it will do nothing.
 *
 * \param[in] list The list to remove the element from
 */
void dll_remh(dlist_t *list);

/**
 * \brief Remove the element in `node` from `list`.
 *
 * \param[in] list The list to remove the element from
 * \param[in] node The node to remove
 */
void dll_rem_node(dlist_t *list, dlnode_t *node);

/**
 * \brief Remove the element at position `index` from `list`.
 *
 * \param[in] list The list to remove the element from
 * \param[in] node The node to remove
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
void dll_rem_index(dlist_t *list, size_t index);

/**
 * \brief Gets the data at position `index` from `list`.
 *
 * \param[in] list The list to get the element from
 * \param[in] index The index of the element
 * \return Pointer to the stored data
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
void *dll_get(dlist_t *list, size_t index);

/**
 * \brief Frees the list and all of the data stores inside.
 *
 * \param[in] list The list to free
 */
void dll_free(dlist_t *list);

/**
 * \brief Creates a new list.
 *
 * \param[in] size [size_t] The size of the stored data
 *
 * \return [dlist_t] A new list instance.
 */
#define DLIST_NEW(size)                                                        \
	(dlist_t){                                                                          \
		NULL, NULL, 0, size                                                    \
	}

/**
 * \brief Gets the data at position `index` from `list` as a `type`.
 *
 * \param[in] list [dlist_t*] The list to get the element from
 * \param[in] type <type> The type of the data stored
 * \param[in] index [size_t] The index of the element
 *
 * \return [type] The data stored at index
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
#define DLIST_GET(list, type, index) (*(type *)dll_get(list, index))

/**
 * \brief Creates an iterator from a Doubly Linked List.
 *
 * \param[in] list The list to iterate
 *
 * \return The created iterator.
 */
iter_t dlist_t_iter_new(dlist_t *list);

/**
 * \brief Creates a reverse iterator from a Doubly Linked List.
 *
 * \param[in] list The list to iterate in reverse
 *
 * \return The created iterator.
 */
iter_t dlist_t_iter_rev(dlist_t *list);

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over a
 * Doubly Linked List.
 *
 * \param[in] iter The iterator to move
 *
 */
void dlist_t_iter_next(iter_t *iter);

/**
 * \brief Moves the iterator `iter` to the previous value, if it iterates over a
 * Doubly Linked List.
 *
 * \param[in] iter The iterator to move
 *
 */
void dlist_t_iter_prev(iter_t *iter);

/**
 * \brief Gets the current data at which `iter` is at, as a `type`, if the
 * iterable is a Doubly Linked List. Since an iterator over a `dlist_t` iterates
 * over nodes, using the normal methods would get the node at which is at and
 * not the value itself.
 *
 * \param[in] iter [iter_t] The iterator to get the value from
 * \param[in] type <type> The type of the stored value
 *
 * \return [type] The current value of the iterator.
 */
#define DLITER_VAL(iter, type) (*(type *)(ITER_VAL(iter, dlnode_t).data))


#endif // !__DLIST__H__
