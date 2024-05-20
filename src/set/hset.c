/**
 * \file hset.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-14
 * \brief HashSet
 */
#include "hset.h"
#include "../err.h"
static int __hs_resize(hset_t *hset)
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
		new_lists[i] = sll_new(hset->data_proto);
	}

	for (size_t idx = 0; idx < hset->capacity; idx++) {
		for_iter(list_t, i, hset->lists + idx)
		{
			hash_t hash = hset->data_proto->hash(ITER_VAL(i, lnode_t).data);
			size_t id = hash % next_size;
			sll_addt(&new_lists[id], ITER_VAL(i, lnode_t).data);
		}
		sll_free(hset->lists + idx);
	}
	free(hset->lists);
	hset->lists = new_lists;
	hset->capacity = next_size;

	return 1;
}

hset_t hs_new(const prototype_t *data_proto)
{
#define INITIAL_CAPACITY 10
	hset_t hset;
	hset.capacity = INITIAL_CAPACITY;

	hset.data_proto = data_proto;
	hset.length = 0;

	hset.lists = malloc(sizeof(list_t) * INITIAL_CAPACITY);
	DIE(!hset.lists, "Malloc Fail");
	for (int i = 0; i < INITIAL_CAPACITY; i++) {
		// TODO: perhaps a list of refs so pointers to elements stay constant
		hset.lists[i] = sll_new(data_proto);
	}
	return hset;
#undef INITIAL_CAPACITY
}

void hs_add(void *hset, void *data)
{
	hset_t *hset_cast = (hset_t *)hset;

	hash_t hash = hset_cast->data_proto->hash(data);
	size_t idx = hash % hset_cast->capacity;
	for_iter(list_t, i, hset_cast->lists + idx)
	{
		if (hset_cast->data_proto->cmp(ITER_VAL(i, lnode_t).data, data) == 0)
			return;
	}
	hset_cast->length++;
	if (__hs_resize(hset_cast)) {
		idx = hash % hset_cast->capacity;
	}
	sll_addh(hset_cast->lists + idx, data);
}

void hs_remove(void *hset, void *data)
{
	hset_t *hset_cast = (hset_t *)hset;

	hash_t hash = hset_cast->data_proto->hash(data);
	size_t idx = hash % hset_cast->capacity;
	lnode_t *prev = NULL;
	for_iter(list_t, i, hset_cast->lists + idx)
	{
		if (hset_cast->data_proto->cmp(ITER_VAL(i, lnode_t).data, data) == 0) {
			lnode_t *prev_ref = prev ? prev : NULL;
			// hset_cast->data_proto->free(ITER_VAL(i, lnode_t).data);
			sll_rem_after(hset_cast->lists + idx, prev_ref);
			hset_cast->length--;
			__hs_resize(hset_cast);
			return;
		}
		prev = &ITER_VAL(i, lnode_t);
	}
}

size_t hs_has(void *hset, void *data)
{
	hset_t *hset_cast = (hset_t *)hset;
	hash_t hash = hset_cast->data_proto->hash(data);
	size_t idx = hash % hset_cast->capacity;
	for_iter(list_t, i, hset_cast->lists + idx)
	{
		if (hset_cast->data_proto->cmp(ITER_VAL(i, lnode_t).data, data) == 0) {
			return 1;
		}
	}
	return 0;
}

void hs_free(hset_t *hset)
{
	for (size_t idx = 0; idx < hset->capacity; idx++) {
		sll_free(hset->lists + idx);
	}
	free(hset->lists);
}

#define hset_t_set_add hs_add
#define hset_t_set_remove hs_remove
#define hset_t_set_has hs_has

/**
 * \brief Creates an iterator from a Singly Linked List.
 *
 * \param[in] list The list to iterate
 *
 * \return The created iterator.
 */
static inline iter_t hset_t_iter_new(void *hset)
{
	lnode_t *first = NULL;
	lnode_t *next = NULL;
	size_t i = 0;
	for (i = 0; i < ((hset_t *)hset)->capacity; i++) {
		lnode_t *elem = ((hset_t *)hset)->lists[i].head;
		if (!elem)
			continue;
		first = elem;
		// printf("FIRST FOUND: %d\n", *(uint32_t *)elem->data);
		next = elem->next;
		// printf("NEXT REF: %p\n", next);

		while (!next) {
			// printf("NO NEXT, NEXT BUCKET\n");

			i++;
			if (i >= ((hset_t *)hset)->capacity) {
				// printf("NO CAPACITY\n");
				break;
			}

			next = ((hset_t *)hset)->lists[i].head;
			// printf("NEXT REF: %p\n", next);
		}
		break;
	}

	return (iter_t){
		.current = first, .iterable = hset, .metadata = i, .next = next};
}
/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over a
 * Singly Linked List.
 *
 * \param[in] iter The iterator to move
 *
 */
static void hset_t_iter_next(iter_t *iter)
{
	iter->current = iter->next;
	if (!iter->current)
		return;

	lnode_t *next = ((lnode_t *)iter->current)->next;
	// printf("NEXT REF: %p\n", next);

	while (!next) {
		// printf("NO NEXT, NEXT BUCKET\n");

		iter->metadata++;
		// printf("NEW BUCKET: %d\n", iter->metadata);

		if (iter->metadata >= ((hset_t *)iter->iterable)->capacity) {
			// printf("NO CAPACITY %d\n", ((hset_t *)iter->iterable)->capacity);
			break;
		}

		next = ((hset_t *)iter->iterable)->lists[iter->metadata].head;
		// printf("NEXT REF: %p\n", next);
	}
	iter->next = next;
}

static void *hset_t_iter_get(iter_t *iter)
{
	return ((lnode_t *)iter->current)->data;
}

DEFINE_PROTO(hset_t, SET);