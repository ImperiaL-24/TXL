/**
 * Copyright (c) 2024
 * \file hmap.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Unordered HashMap
 */
#include "hmap.h"
#include "../err.h"
#include "../impl/iter.h"
#include "kv_pair.h"
/**
 * \brief Resizes a hashmap if the load factor is too large.
 *
 * \param hmap The hmap to resize.
 * \return `1` If a resize was done. `0` otherwise.
 */
static int __hm_resize(hmap_t *hmap)
{
	float load;
	size_t next_size;
	size_t occupied_buckets = 0;
	for (size_t i = 0; i < hmap->capacity; i++) {
		if (hmap->lists[i].size != 0)
			occupied_buckets++;
	}
	load = (float)occupied_buckets / (float)hmap->capacity;

	/* If we need to grow */
	if (load > 0.72) {
		next_size = hmap->capacity * 2;
		/* If we need to shrink (but not too much) */
	} else if (load < 0.3 && hmap->capacity > 10) {
		next_size = hmap->capacity / 2;
	} else {
		return 0;
	}
	list_t *new_lists = malloc(next_size * sizeof(list_t));
	DIE(!new_lists, "Malloc Fail");
	for (size_t i = 0; i < next_size; i++) {
		new_lists[i] = LIST_NEW(kv_pair_t);
	}

	for (size_t idx = 0; idx < hmap->capacity; idx++) {
		for_iter(list_t, i, hmap->lists + idx)
		{
			hash_t hash = hmap->key_proto->hash(SLITER_VAL(i, kv_pair_t).key);
			size_t id = hash % next_size;
			sll_addt(&new_lists[id], &SLITER_VAL(i, kv_pair_t));
		}
		sll_free(hmap->lists + idx);
	}
	free(hmap->lists);
	hmap->lists = new_lists;
	hmap->capacity = next_size;

	return 1;
}

hmap_t hm_new(const prototype_t *key_proto, const prototype_t *value_proto)
{
#define INITIAL_CAPACITY 10
	hmap_t hmap;
	hmap.capacity = INITIAL_CAPACITY;

	hmap.key_proto = key_proto;
	// DIE((~key_proto->trait_code & 0b1111) != 0, "Passed type cannot be a
	// key!"); hmap.hash_key = hash_function; hmap.cmp_key = cmp_function;

	hmap.value_proto = value_proto;
	// DIE((~value_proto->trait_code & 0b1100) != 0,
	// 	"Passed type cannot be a key!");

	hmap.length = 0;

	hmap.lists = malloc(sizeof(list_t) * INITIAL_CAPACITY);
	DIE(!hmap.lists, "Malloc Fail");
	for (int i = 0; i < INITIAL_CAPACITY; i++) {
		hmap.lists[i] = LIST_NEW(kv_pair_t);
	}
	return hmap;
#undef INITIAL_CAPACITY
}

void hm_set(hmap_t *hmap, void *key, void *value)
{
	hash_t hash = hmap->key_proto->hash(key);
	size_t idx = hash % hmap->capacity;
	kv_pair_t pair =
		kv_pair_new(key, value, hmap->key_proto, hmap->value_proto);
	for_iter(list_t, i, hmap->lists + idx)
	{
		if (hmap->key_proto->cmp(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
			kv_pair_deep_free(SLITER_VAL(i, kv_pair_t), hmap->key_proto,
							  hmap->value_proto);
			memcpy(&SLITER_VAL(i, kv_pair_t), &pair, sizeof(kv_pair_t));
			return;
		}
	}
	hmap->length++;
	if (__hm_resize(hmap)) {
		idx = hash % hmap->capacity;
	}
	sll_addh(hmap->lists + idx, &pair);
}

void hm_remove(hmap_t *hmap, void *key)
{
	hash_t hash = hmap->key_proto->hash(key);
	size_t idx = hash % hmap->capacity;
	lnode_t *prev = NULL;
	for_iter(list_t, i, hmap->lists + idx)
	{
		if (hmap->key_proto->cmp(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
			lnode_t *prev_ref = prev ? prev : NULL;
			kv_pair_data_free(SLITER_VAL(i, kv_pair_t), hmap->key_proto,
							  hmap->value_proto);
			sll_rem_after(hmap->lists + idx, prev_ref);
			hmap->length--;
			__hm_resize(hmap);
			return;
		}
		prev = &ITER_VAL(i, lnode_t);
	}
}

void *hm_get(hmap_t *hmap, void *key)
{
	hash_t hash = hmap->key_proto->hash(key);
	size_t idx = hash % hmap->capacity;
	for_iter(list_t, i, hmap->lists + idx)
	{
		if (hmap->key_proto->cmp(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
			return SLITER_VAL(i, kv_pair_t).value;
		}
	}
	return NULL;
}

size_t hm_has(hmap_t *hmap, void *key)
{
	hash_t hash = hmap->key_proto->hash(key);
	size_t idx = hash % hmap->capacity;
	for_iter(list_t, i, hmap->lists + idx)
	{
		if (hmap->key_proto->cmp(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
			return 1;
		}
	}
	return 0;
}

void hm_free(hmap_t *hmap)
{
	for (size_t idx = 0; idx < hmap->capacity; idx++) {
		for_iter(list_t, i, hmap->lists + idx)
		{
			kv_pair_data_free(SLITER_VAL(i, kv_pair_t), hmap->key_proto,
							  hmap->value_proto);
		}
		sll_free(hmap->lists + idx);
	}
	free(hmap->lists);
}
