/**
 * \file tset.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-21
 * \brief Tree Set
 */

#include "tset.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PRIORITY 1000

tset_t ts_new(const prototype_t *data_proto)
{
	srand(time(NULL));
	return (tset_t){.data_proto = data_proto, .root = NULL};
}

static void __tsnode_free(tsnode_t *node, const prototype_t *data_proto)
{
	if (data_proto->free)
		data_proto->free(node);
	free(node->data);
	free(node);
}

static void __subtree_free(tsnode_t *node, const prototype_t *data_proto)
{
	if (!node) {
		return;
	}
	__subtree_free(node->left, data_proto);
	__subtree_free(node->right, data_proto);
	__tsnode_free(node, data_proto);
}

void ts_free(tset_t *tset)
{
	if (tset->root != NULL) {
		__subtree_free(tset->root, tset->data_proto);
	}
}

/* Creeaza un nod
 * @param1: Valoarea ce trebuie pusa in nod.
 * @param2: Numarul de octeti pe care scrie valoarea.
 */
tsnode_t *__tsnode_create(const prototype_t *data_proto, void *value,
						  tsnode_t *parent)
{
	tsnode_t *node = malloc(sizeof(tsnode_t));
	if (node == NULL) {
		return NULL;
	}
	node->data = malloc(data_proto->size);
	data_proto->clone(node->data, value);

	node->priority = rand() % MAX_PRIORITY;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	return node;
}

int priority(tsnode_t *node)
{
	if (node == NULL) {
		return -1;
	}
	return node->priority;
}

void __rotate_right(tsnode_t **node)
{
	if (*node == NULL || (*node)->left == NULL) {
		return;
	}
	tsnode_t *lson = (*node)->left;

	lson->parent = (*node)->parent;
	(*node)->parent = lson;

	if (lson->right)
		lson->right->parent = *node;

	(*node)->left = lson->right;
	lson->right = *node;
	*node = lson;
}

void __rotate_left(tsnode_t **node)
{
	if (*node == NULL || (*node)->right == NULL) {
		return;
	}
	tsnode_t *rson = (*node)->right;

	rson->parent = (*node)->parent;
	(*node)->parent = rson;

	if (rson->left)
		rson->left->parent = *node;

	(*node)->right = rson->left;
	rson->left = *node;
	*node = rson;
}

/* Inserare in Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
static void __ts_insert(tsnode_t **node, void *value,
						const prototype_t *data_proto, tsnode_t *parent)
{
	if (*node == NULL) {
		*node = __tsnode_create(data_proto, value, parent);
		return;
	}
	if (data_proto->cmp(value, (*node)->data) < 0) {
		__ts_insert(&(*node)->left, value, data_proto, *node);
		if (priority((*node)->left) > priority(*node)) {
			__rotate_right(node);
		}
	} else {
		__ts_insert(&(*node)->right, value, data_proto, *node);
		if (priority((*node)->right) > priority(*node)) {
			__rotate_left(node);
		}
	}
}
void ts_add(void *tset, void *data)
{
	if (ts_has(tset, data))
		return;
	tset_t *tset_cast = (tset_t *)tset;
	__ts_insert(&tset_cast->root, data, tset_cast->data_proto, NULL);
}

/* Stergere din Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
static void __ts_delete(tsnode_t **node, void *value,
						const prototype_t *data_proto)
{
	if (*node == NULL) {
		return;
	}
	if (data_proto->cmp(value, (*node)->data) < 0) {
		__ts_delete(&(*node)->left, value, data_proto);
		return;
	}
	if (data_proto->cmp(value, (*node)->data) > 0) {
		__ts_delete(&(*node)->right, value, data_proto);
		return;
	}
	if ((*node)->left == NULL && (*node)->right == NULL) {
		__tsnode_free(*node, data_proto);

		*node = NULL;
		return;
	}

	if (priority((*node)->left) > priority((*node)->right)) {
		__rotate_right(node);
		__ts_delete(&(*node)->right, value, data_proto);
		return;
	}
	__rotate_left(node);
	__ts_delete(&(*node)->left, value, data_proto);
}

void ts_remove(void *tset, void *data)
{
	if (!ts_has(tset, data)) // TODO: check if necessary
		return;
	tset_t *tset_cast = (tset_t *)tset;
	__ts_delete(&tset_cast->root, data, tset_cast->data_proto);
}

size_t __ts_search(tsnode_t *node, void *value, const prototype_t *data_proto)
{
	if (node == NULL) {
		return 0;
	}
	if (data_proto->cmp(value, node->data) < 0) {
		return __ts_search(node->left, value, data_proto);
	}
	if (data_proto->cmp(value, node->data) > 0) {
		return __ts_search(node->right, value, data_proto);
	}
	return 1;
}

size_t ts_has(void *tset, void *data)
{
	tset_t *tset_cast = (tset_t *)tset;
	return __ts_search(tset_cast->root, data, tset_cast->data_proto);
}

int is_left_child(tsnode_t *node, const prototype_t *data_proto)
{
	if (node->parent == NULL)
		return 0;
	if (node->parent->left == NULL)
		return 0;
	if (data_proto->cmp(node->parent->left->data, node->data) == 0)
		return 1;
	return 0;
}

int is_right_child(tsnode_t *node, const prototype_t *data_proto)
{
	if (node->parent == NULL)
		return 0;
	if (node->parent->right == NULL)
		return 0;
	if (data_proto->cmp(node->parent->right->data, node->data) == 0)
		return 1;
	return 0;
}

// #define NF "(V: %d, L: %d, R: %d)"
// #define PR(node) (node ? *(uint32_t *)node->data : 666)
// #define PRF(node) PR(node), PR(node->left), PR(node->right)
static inline tsnode_t *__next_node(tset_t *tset, tsnode_t *curr)
{
	// printf("CALCULATING NEXT\n");
	// if (curr)
	// printf("CURR:" NF "\n", PRF(curr));
	// if (curr && curr->parent)
	// printf("PRNT:" NF "\n", PRF(curr->parent));

	if (!curr)
		return NULL;
	if (curr->right) {
		// printf("CASE 1 ");

		tsnode_t *succ = curr->right;
		while (succ->left)
			succ = succ->left;
		// printf(": %d\n", *(uint32_t *)succ->data);
		return succ;
	}

	if (!curr->parent) {
		return NULL;
	}

	if (is_left_child(curr, tset->data_proto)) {
		// printf("CASE 2\n");
		//
		return curr->parent;
	}

	// TODO: check if if is useless
	// if (is_right_child(curr, tset->data_proto)) {
	// printf("CASE 3\n");

	tsnode_t *pre = curr->parent;
	while (pre->parent) {
		if (is_left_child(pre, tset->data_proto)) {
			return pre->parent;
		}
		pre = pre->parent;
	}
	// }
	// printf("NO NEXT\n");

	return NULL;
}

/**
 * \brief Creates an iterator from a Singly Linked List.
 *
 * \param[in] list The list to iterate
 *
 * \return The created iterator.
 */
static inline iter_t tset_t_iter_new(void *tset)
{
	tsnode_t *curr = ((tset_t *)tset)->root;
	while (curr && curr->left) {
		curr = curr->left;
	}
	return (iter_t){
		.current = curr, .iterable = tset, .next = __next_node(tset, curr)};
}

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over a
 * Singly Linked List.
 *
 * \param[in] iter The iterator to move
 *
 */
static void tset_t_iter_next(iter_t *iter)
{
	iter->current = iter->next;
	iter->next = __next_node(iter->iterable, iter->current);
}

static void *tset_t_iter_get(iter_t *iter)
{
	return ((tsnode_t *)iter->current)->data;
}

#define tset_t_set_add ts_add
#define tset_t_set_remove ts_remove
#define tset_t_set_has ts_has

DEFINE_PROTO(tset_t, SET);