/**
 * \file dlist.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-22
 * \brief Doubly-Linked List Implementation
 */
#include "dlist.h"

/**
 * \brief Inserts a node between 2 other nodes.
 *
 * \param[in] prev The previous node reference
 * \param[in] next The next node reference
 * \param[in] node The node to add
 *
 * \note This is for internal use only. `prev` and `next` are addresses of node
 * references, like `&dlnode_t.next` and `&dlist_t.head`.
 */
static inline void __dlnode_insert(dlnode_t **prev, dlnode_t **next,
								   dlnode_t *node)
{
	node->next = *prev;
	node->prev = *next;
	*prev = node;
	*next = node;
}

/**
 * \brief Safely inserts `node` in a node reference.
 *
 * \param[in] list The list to add the node into
 * \param[in] prev The previous node reference
 * \param[in] node The node to insert
 *
 * \note This is for internal use only. `prev` is an address of a node
 * reference, like `&dlnode_t.next` and `&dlist_t.head`.
 */
static inline void __dlnode_safe_insert(dlist_t *list, dlnode_t **prev,
										dlnode_t *node)
{
	if (*prev)
		__dlnode_insert(prev, &(*prev)->prev, node);
	else
		__dlnode_insert(prev, &list->tail, node);
	list->size++;
}

/**
 * \brief Safely remove a node from `list`. This does not free the node or the
 * data inside.
 *
 * \param[in] list The list to remove from
 * \param[in] node The node to remove
 *
 * \note This is for internal use only.
 */
static inline void __dlnode_safe_remove(dlist_t *list, dlnode_t *node)
{
	dlnode_t **next = node->next ? &node->next->prev : &list->tail;
	dlnode_t **prev = node->prev ? &node->prev->next : &list->head;
	*prev = node->next;
	*next = node->prev;
	list->size--;
}


inline void dlnode_free(dlnode_t *node)
{
	free(node->data);
	free(node);
}

/**
 * \brief Creates an `iter_t` from a `list` and a starting node.
 *
 * \param[in] list The list to iterate from
 * \param[in] start The starting node
 *
 * \return The created iterator.
 */
static inline iter_t iter_from(dlist_t *list, dlnode_t *start)
{
	return (iter_t){.current = start, .iterable = list};
}

inline iter_t dlist_t_iter_new(dlist_t *list)
{
	return iter_from(list, list->head);
}

inline iter_t dlist_t_iter_rev(dlist_t *list)
{
	return iter_from(list, list->tail);
}

void dlist_t_iter_next(iter_t *iter)
{
	iter->current = ((dlnode_t *)iter->current)->next;
}

void dlist_t_iter_prev(iter_t *iter)
{
	iter->current = ((dlnode_t *)iter->current)->prev;
}


/**
 * \brief Jumps to the node at position `index` from `list`.
 *
 * \param[in] list The list to get the node from
 * \param[in] index The index of the node to get
 *
 * \return A reference to the node at position `index` or `NULL` if there is no
 * element at that index.
 *
 * \note This is for internal use only.
 */
static dlnode_t *dll_jump(dlist_t *list, size_t index)
{
	/* Find out if it is faster to iterate from the front or back */
	if (index > list->size / 2) {
		size_t iter_index = list->size - 1;
		for_iter_rev(dlist_t, i, list) {
			if (iter_index == index || !ITER_VAL(i, dlnode_t).prev)
				return &ITER_VAL(i, dlnode_t);

			iter_index--;
		}
	} else {
		size_t iter_index = 0;
		for_iter(dlist_t, i, list) {
			if (iter_index == index || !ITER_VAL(i, dlnode_t).next)
				return &ITER_VAL(i, dlnode_t);

			iter_index++;
		}
	}

	return NULL;
}

/**
 * \brief Creates a new node that stores `dat`. Copies the data into the node.
 *
 * \param[in] dat_size The size of the data
 * \param[in] dat The data to store
 * \return The created node
 *
 * \note This is for internal use only.
 */
static dlnode_t *dlnode_new(size_t dat_size, void *dat)
{
	void *data = malloc(dat_size);
	memcpy(data, dat, dat_size);

	dlnode_t *node = malloc(sizeof(dlnode_t));
	*node = (dlnode_t){.data = data, .next = NULL, .prev = NULL};

	return node;
}

inline void dll_addh(dlist_t *list, void *data)
{
	dll_add_index(list, 0, data);
}

inline void dll_addt(dlist_t *list, void *data)
{
	if (!list->size)
		dll_addh(list, data);
	else
		dll_add_after(list, list->tail, data);
}

void dll_add_after(dlist_t *list, dlnode_t *node, void *data)
{
	dlnode_t *lnode = dlnode_new(list->data_size, data);
	__dlnode_safe_insert(list, &node->next, lnode);
}

void dll_add_index(dlist_t *list, size_t index, void *data)
{
	dlnode_t *lnode = dlnode_new(list->data_size, data);
	if (!index || !list->size) {
		__dlnode_safe_insert(list, &list->head, lnode);
		return;
	}
	dlnode_t *prev = dll_jump(list, index - 1);
	__dlnode_safe_insert(list, &prev->next, lnode);
}

inline void dll_remh(dlist_t *list)
{
	if (!list->size)
		return;
	dll_rem_index(list, 0);
}

void dll_rem_node(dlist_t *list, dlnode_t *node)
{
	if (!list->size)
		return;
	__dlnode_safe_remove(list, node);
	dlnode_free(node);
}

void dll_rem_index(dlist_t *list, size_t index)
{
	if (!list->size)
		return;
	dlnode_t *node = dll_jump(list, index);
	__dlnode_safe_remove(list, node);
	dlnode_free(node);
}

inline void *dll_get(dlist_t *list, size_t index)
{
	if (!index && list->size)
		return list->head->data;
	return dll_jump(list, index)->data;
}

void dll_free(dlist_t *list)
{
	for_iter(dlist_t, i, list)
	{
		if (&ITER_VAL(i, dlnode_t) == list->head)
			continue;
		dlnode_free(ITER_VAL(i, dlnode_t).prev);
	}
	dlnode_free(list->tail);
}
