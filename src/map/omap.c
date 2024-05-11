/**
 * Copyright (c) 2024
 * \file omap.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Ordered HashMap
 */
#include "omap.h"
#include "../impl/ref.h"

omap_t om_new(prototype_t *key_proto, prototype_t *value_proto)
{
	omap_t map;
	map.access = hm_new(key_proto, PROTOTYPE(ref_t));
	map.data = DLIST_NEW(kv_pair_t);
	map.value_proto = value_proto;
	return map;
}

void om_set(omap_t *omap, void *key, void *value)
{
	dlnode_t **node_ref = hm_get(&omap->access, key);
	kv_pair_t pair =
		kv_pair_new(key, value, omap->access.key_proto, omap->value_proto);
	if (!node_ref) {
		dll_addt(&omap->data, &pair);
		hm_set(&omap->access, key, &omap->data.tail);
		return;
	}
	kv_pair_t *value_loc = (kv_pair_t *)(*node_ref)->data;
	kv_pair_deep_free(*value_loc, omap->access.key_proto, omap->value_proto);
	omap->data.data_proto->clone(value_loc, &pair);
	// memmove(value_loc, &pair, sizeof(kv_pair_t));
	dlnode_t *node = *node_ref;
	hm_remove(&omap->access, key);

	hm_set(&omap->access, key, &node);
}

void om_remove(omap_t *omap, void *key)
{
	dlnode_t **node = hm_get(&omap->access, key);
	if (!node)
		return;

	kv_pair_t *value_loc = (kv_pair_t *)(*node)->data;
	kv_pair_data_free(*value_loc, omap->access.key_proto, omap->value_proto);

	dll_rem_node(&omap->data, *node);
	hm_remove(&omap->access, key);
}

void om_remove_first(omap_t *omap)
{
	if (!omap->data.size)
		return;
	kv_pair_t pair = *(kv_pair_t *)omap->data.head->data;
	hm_remove(&omap->access, pair.key);
	dll_remh(&omap->data);

	kv_pair_data_free(pair, omap->access.key_proto, omap->value_proto);
}

inline void *om_get(omap_t *omap, void *key)
{
	dlnode_t **node_ref = hm_get(&omap->access, key);

	return node_ref ? ((kv_pair_t *)(*node_ref)->data)->value : NULL;
}

inline kv_pair_t *om_get_first(omap_t *omap)
{
	if (!omap->data.head)
		return NULL;
	return (kv_pair_t *)omap->data.head->data;
}

inline size_t om_has(omap_t *omap, void *key)
{
	return hm_has(&omap->access, key);
}

inline iter_t omap_t_iter_new(omap_t *omap)
{
	return dlist_t_iter_new(&omap->data);
}

inline iter_t omap_t_iter_rev(omap_t *omap)
{
	return dlist_t_iter_rev(&omap->data);
}

inline void omap_t_iter_next(iter_t *iter) { dlist_t_iter_next(iter); }

inline void omap_t_iter_prev(iter_t *iter) { dlist_t_iter_prev(iter); }

void om_free(omap_t *omap)
{
	hm_free(&omap->access);
	for_iter(dlist_t, i, &omap->data)
	{
		kv_pair_data_free(DLITER_VAL(i, kv_pair_t), omap->access.key_proto,
						  omap->value_proto);
	}
	dll_free(&omap->data);
}
