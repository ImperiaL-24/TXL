/**
 * \file list.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Singly Linled List
 */
#ifndef __LIST__H__
#define __LIST__H__
#include <stdlib.h>
#include <string.h>
#include "../impl/iter.h"
/**
 * \brief A Singly Linked List node. Represents a location in a `list_t`.
 */
typedef struct lnode_t {
	void *data;
	struct lnode_t *next;
} lnode_t;

/**
 * \brief A generic doubly linked list. `list_t` is a One Way Iterable object.
 */
typedef struct {
	lnode_t *head;
	lnode_t *tail;
	size_t size;
	size_t data_size;
} list_t;

/**
 * \brief Add `data` to the start of the `list`.
 *
 * \param[in] list The list to add to
 * \param[in] data The data to add
 */
void sll_addh(list_t *list, void *data);

/**
 * \brief Add `data` to the end of the `list`.
 *
 * \param[in] list The list to add to
 * \param[in] data The data to add
 */
void sll_addt(list_t *list, void *data);

/**
 * \brief Add `data` after a `node` from `list`.
 *
 * \param[in] list The list to add to
 * \param[in] node The node to add after
 * \param[in] data The data to add
 */
void sll_add_after(list_t *list, lnode_t *node, void *data);

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
void sll_add_index(list_t *list, size_t index, void *data);

/**
 * \brief Removes the first element from a list. If there are no elements in the
 * list then it will do nothing.
 *
 * \param[in] list The list to remove the element from
 */
void sll_remh(list_t *list);

void sll_rem_after(list_t *list, lnode_t* prev);

/**
 * \brief Remove the element at position `index` from `list`.
 *
 * \param[in] list The list to remove the element from
 * \param[in] node The node to remove
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
void sll_rem_index(list_t *list, size_t index);

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
void *sll_get(list_t *list, size_t index);

/**
 * \brief Frees the list and all of the data stores inside.
 *
 * \param[in] list The list to free
 */
void sll_free(list_t *list);

/**
 * \brief Creates a new list.
 *
 * \param[in] size [size_t] The size of the stored data
 *
 * \return [list_t] A new list instance.
 */
#define LIST_NEW(size)                                                        \
	(list_t){                                                                          \
		NULL, NULL, 0, size                                                    \
	}

/**
 * \brief Gets the data at position `index` from `list` as a `type`.
 *
 * \param[in] list [list_t*] The list to get the element from
 * \param[in] type <type> The type of the data stored
 * \param[in] index [size_t] The index of the element
 *
 * \return [type] The data stored at index
 *
 * \note This searches for the `index` element, so this operation is deemed
 * expensive.
 */
#define LIST_GET(list, type, index) (*(type *)sll_get(list, index))

/**
 * \brief Creates an iterator from a Doubly Linked List.
 *
 * \param[in] list The list to iterate
 *
 * \return The created iterator.
 */
iter_t list_t_iter_new(list_t *list);

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over a
 * Doubly Linked List.
 *
 * \param[in] iter The iterator to move
 *
 */
void list_t_iter_next(iter_t *iter);

/**
 * \brief Gets the current data at which `iter` is at, as a `type`, if the
 * iterable is a Singly Linked List. Since an iterator over a `list_t` iterates
 * over nodes, using the normal methods would get the node at which is at and
 * not the value itself.
 *
 * \param[in] iter [iter_t] The iterator to get the value from
 * \param[in] type <type> The type of the stored value
 *
 * \return [type] The current value of the iterator.
 */
#define SLITER_VAL(iter, type) (*(type *)(ITER_VAL(iter, lnode_t).data))

#endif // !__LIST__H__
