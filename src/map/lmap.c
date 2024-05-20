/**
 * Copyright (c) 2024
 * \file lmap.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Linked HashMap
 */
#include "lmap.h"
#include "../impl/ref.h"

lmap_t lm_new(const prototype_t *key_proto, const prototype_t *value_proto)
{
	lmap_t map;
	map.access = hm_new(key_proto, PROTOTYPE(ref_t));
	map.data = DLIST_NEW(kv_pair_t);
	map.value_proto = value_proto;
	return map;
}

void lm_set(lmap_t *lmap, void *key, void *value)
{
	dlnode_t **node_ref = hm_get(&lmap->access, key);
	kv_pair_t pair =
		kv_pair_new(key, value, lmap->access.key_proto, lmap->value_proto);
	if (!node_ref) {
		dll_addt(&lmap->data, &pair);
		hm_set(&lmap->access, key, &lmap->data.tail);
		return;
	}
	kv_pair_t *value_loc = (kv_pair_t *)(*node_ref)->data;
	kv_pair_deep_free(*value_loc, lmap->access.key_proto, lmap->value_proto);
	lmap->data.data_proto->clone(value_loc, &pair);
	// memmove(value_loc, &pair, sizeof(kv_pair_t));
	dlnode_t *node = *node_ref;
	hm_remove(&lmap->access, key);

	hm_set(&lmap->access, key, &node);
}

void lm_remove(lmap_t *lmap, void *key)
{
	dlnode_t **node = hm_get(&lmap->access, key);
	if (!node)
		return;

	kv_pair_t *value_loc = (kv_pair_t *)(*node)->data;
	kv_pair_data_free(*value_loc, lmap->access.key_proto, lmap->value_proto);

	dll_rem_node(&lmap->data, *node);
	hm_remove(&lmap->access, key);
}

void lm_remove_first(lmap_t *lmap)
{
	if (!lmap->data.size)
		return;
	kv_pair_t pair = *(kv_pair_t *)lmap->data.head->data;
	hm_remove(&lmap->access, pair.key);
	dll_remh(&lmap->data);

	kv_pair_data_free(pair, lmap->access.key_proto, lmap->value_proto);
}

inline void *lm_get(lmap_t *lmap, void *key)
{
	dlnode_t **node_ref = hm_get(&lmap->access, key);

	return node_ref ? ((kv_pair_t *)(*node_ref)->data)->value : NULL;
}

inline kv_pair_t *lm_get_first(lmap_t *lmap)
{
	if (!lmap->data.head)
		return NULL;
	return (kv_pair_t *)lmap->data.head->data;
}

inline size_t lm_has(lmap_t *lmap, void *key)
{
	return hm_has(&lmap->access, key);
}

/**
 * \brief Creates an iterator from an Linked HashMap.
 *
 * \param[in] list The map to iterate
 *
 * \return The created iterator.
 */
iter_t lmap_t_iter_new(void *lmap)
{
	return PROTOTYPE(dlist_t)->iter_new(&((lmap_t *)lmap)->data);
}

/**
 * \brief Creates a reverse iterator from an Linked HashMap.
 *
 * \param[in] list The map to iterate in reverse
 *
 * \return The created iterator.
 */
iter_t lmap_t_iter_rev(void *lmap)
{
	return PROTOTYPE(dlist_t)->iter_rev(&((lmap_t *)lmap)->data);
}

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over
 * an Linked HashMap.
 *
 * \param[in] iter The iterator to move
 *
 */
void lmap_t_iter_next(iter_t *iter) { PROTOTYPE(dlist_t)->iter_next(iter); }

/**
 * \brief Moves the iterator `iter` to the previous value, if it iterates over
 * an Linked HashMap.
 *
 * \param[in] iter The iterator to move
 *
 */
void lmap_t_iter_prev(iter_t *iter) { PROTOTYPE(dlist_t)->iter_prev(iter); }

void lm_free(lmap_t *lmap)
{
	hm_free(&lmap->access);
	for_iter(dlist_t, i, &lmap->data)
	{
		kv_pair_data_free(DLITER_VAL(i, kv_pair_t), lmap->access.key_proto,
						  lmap->value_proto);
	}
	dll_free(&lmap->data);
}

static void *lmap_t_iter_get(iter_t *iter)
{
	return ((dlnode_t *)iter->current)->data;
}

DEFINE_PROTO(lmap_t, ITER_FULL);