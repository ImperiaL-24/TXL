/**
 * \file hset.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-14
 * \brief HashSet
 */
#include "hset.h"

// TODO: redo resizing
static int __hm_resize(hset_t *hset)
{
	float load;
	size_t next_size;
	size_t occupied_buckets = 0;
	for (size_t i = 0; i < hset->capacity; i++) {
		if (hset->lists[i].size != 0)
			occupied_buckets++;
	}
	load = (float)occupied_buckets / (float)hset->capacity;

	/* If we need to grow */
	if (load > 0.72) {
		next_size = hset->capacity * 2;
		/* If we need to shrink (but not too much) */
	} else if (load < 0.3 && hset->capacity > 10) {
		next_size = hset->capacity / 2;
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

hset_t hs_new(prototype_t *data_proto)
{
#define INITIAL_CAPACITY 10
	hset_t hset;
	hset.capacity = INITIAL_CAPACITY;

	hset.data_proto = data_proto;
	hset.length = 0;

	hset.lists = malloc(sizeof(list_t) * INITIAL_CAPACITY);
	DIE(!hset.lists, "Malloc Fail");
	for (int i = 0; i < INITIAL_CAPACITY; i++) {
		hset.lists[i] = sll_new(data_proto);
	}
	return hset;
#undef INITIAL_CAPACITY
}

void hs_add(hset_t *hset, void *data);

void hs_remove(hset_t *hset, void *data);

void hs_has(hset_t *hset, void *data);

DEFINE_PROTO(hset_t, SET);