/**
 * \file list.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Singly Linled List Implementation
 */
#include "list.h"

/**
 * \brief Inserts a node after other node reference.
 *
 * \param[in] prev The previous node reference
 * \param[in] node The node to add
 *
 * \note This is for internal use only. `prev` is an address of node
 * references, like `&slnode_t.next` and `&list_t.head`.
 */
static inline void __lnode_insert(lnode_t **prev,
								   lnode_t *node)
{
	node->next = *prev;
	// node->prev = *next;
	*prev = node;
	// *next = node;
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
static inline void __lnode_safe_insert(list_t *list, lnode_t **prev,
										lnode_t *node)
{
	if (!*prev)
		list->tail = node;
	
	__lnode_insert(prev, node);
	list->size++;
}

/**
 * \brief Safely remove the node after `prev` from `list`. This does not free the node or the
 * data inside.
 *
 * \param[in] list The list to remove from
 * \param[in] node The node to remove
 *
 * \note This is for internal use only.
 */
static inline void __lnode_safe_remove(list_t *list, lnode_t *prev)
{
	lnode_t **prev_ref = prev ? &prev->next : &list->head;
	if(!(*prev_ref)->next)
		list->tail = prev;
	*prev_ref = (*prev_ref)->next;
	list->size--;
}

/**
 * \brief Frees a node and the data inside.
 *
 * \param[in] node The node to free
 *
 * \note This is for internal use only.
 */
static inline void __lnode_free(lnode_t *node)
{
	free(node->data);
	free(node);
}

inline iter_t list_t_iter_new(list_t *list)
{
	return (iter_t){.current = list->head, .iterable = list};
}


void list_t_iter_next(iter_t *iter)
{
	iter->current = ((lnode_t *)iter->current)->next;
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
static lnode_t *sll_jump(list_t *list, size_t index)
{

	size_t iter_index = 0;
	for_iter(list_t, i, list)
	{
		if (iter_index == index || !ITER_VAL(i, lnode_t).next)
			return &ITER_VAL(i, lnode_t);

		iter_index++;
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
static lnode_t *lnode_new(size_t dat_size, void *dat)
{
	void *data = malloc(dat_size);
	memcpy(data, dat, dat_size);

	lnode_t *node = malloc(sizeof(lnode_t));
	*node = (lnode_t){.data = data, .next = NULL};

	return node;
}

inline void sll_addh(list_t *list, void *data)
{
	sll_add_index(list, 0, data);
}

inline void sll_addt(list_t *list, void *data)
{
	if (!list->size)
		sll_addh(list, data);
	else
		sll_add_after(list, list->tail, data);
}

void sll_add_after(list_t *list, lnode_t *node, void *data)
{
	lnode_t *lnode = lnode_new(list->data_size, data);
	__lnode_safe_insert(list, &node->next, lnode);
}

void sll_add_index(list_t *list, size_t index, void *data)
{
	lnode_t *lnode = lnode_new(list->data_size, data);
	if (!index || !list->size) {
		__lnode_safe_insert(list, &list->head, lnode);
		return;
	}
	lnode_t *prev = sll_jump(list, index - 1);
	__lnode_safe_insert(list, &prev->next, lnode);
}

inline void sll_remh(list_t *list)
{
	if (!list->size)
		return;
	sll_rem_index(list, 0);
}

void sll_rem_after(list_t *list, lnode_t* prev)
{
	if(!prev) {
		lnode_t* node = list->head;
		__lnode_safe_remove(list, NULL);
		__lnode_free(node);
		return;
	}

	if (!list->size)
		return;
	lnode_t* node = prev->next;
	if(!node)
		return;
	__lnode_safe_remove(list, prev);
	__lnode_free(node);
}

void sll_rem_index(list_t *list, size_t index)
{
	if (!list->size)
		return;
	if(!index) {
		lnode_t* node = list->head;
		__lnode_safe_remove(list, NULL);
		__lnode_free(node);
		return;
	}
	size_t jmp_idx = index >= list->size ? list->size - 2 : index - 1;
	lnode_t *prev = sll_jump(list, jmp_idx);
	lnode_t* node = prev->next;
	if(!node) 
		return;
	__lnode_safe_remove(list, prev);
	__lnode_free(node);
}

inline void *sll_get(list_t *list, size_t index)
{
	if (!index && list->size)
		return list->head->data;
	return sll_jump(list, index)->data;
}

void sll_free(list_t *list)
{	
	/* for_iter requires iterated elements still exist. So freeing and using a for_iter cannot be done. */
	lnode_t* current = list->head;
	while (current) {	
		lnode_t* nxt = current->next;
		__lnode_free(current);
		current = nxt;
	}
}
