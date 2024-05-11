/**
 * \file kv_pair.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-11
 * \brief Key Value Pair
 */

#include "kv_pair.h"
#include "../impl/clone.h"

kv_pair_t kv_pair_new(void *key, void *value, prototype_t *key_proto,
					  prototype_t *value_proto)
{
	void *new_data = malloc(value_proto->size);
	value_proto->clone(new_data, value);
	void *new_key = malloc(key_proto->size);
	key_proto->clone(new_key, key);

	kv_pair_t pair;
	pair.key = new_key;
	pair.value = new_data;
	return pair;
}

inline void kv_pair_data_free(kv_pair_t pair, prototype_t *key_proto,
							  prototype_t *value_proto)
{
	if (key_proto->free)
		key_proto->free(pair.key);
	if (value_proto->free)
		value_proto->free(pair.value);
}

void kv_pair_deep_free(kv_pair_t pair, prototype_t *key_proto,
					   prototype_t *value_proto)
{
	kv_pair_data_free(pair, key_proto, value_proto);
	free(pair.key);
	free(pair.value);
}

static void kv_pair_t_free(void *pair)
{
	kv_pair_t *pair_cast = (kv_pair_t *)pair;
	free(pair_cast->key);
	free(pair_cast->value);
}

impl_default_clone(kv_pair_t);

DEFINE_PROTO(kv_pair_t, FREE, CLONE);
