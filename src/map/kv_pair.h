/**
 * \file kv_pair.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-11
 * \brief Key-Value Pair
 */
#ifndef __KV_PAIR__H__
#define __KV_PAIR__H__

#include "../impl/proto.h"

/**
 * \brief A key-value pair. It does not own the data, as it would need to store
 * 2 prototype pointers. Because of this, it has severe limitations.
 */
typedef struct {
	void *key;
	void *value;
} kv_pair_t;

/**
 * \brief Creates a new key-value pair. This deep copies `key` and `value`.
 * Since `kv_pair_t` does not own this data, it will have to be manually freed
 * or call `kv_pair_deep_free()` for a deep free.
 *
 *
 * \param key The key to add
 * \param value The value to add
 * \param key_proto The prototype of the key
 * \param value_proto The prototype of the value
 *
 * \return The created key-value pair.
 */
kv_pair_t kv_pair_new(void *key, void *value, prototype_t *key_proto,
					  prototype_t *value_proto);

/**
 * \brief Deep Frees a key-value pair.
 *
 * \param pair The pair to free
 * \param free_key How to free the key
 * \param free_value How to free the value
 *
 */
void kv_pair_deep_free(kv_pair_t pair, prototype_t *key_proto,
					   prototype_t *value_proto);
/**
 * \brief Frees the data held by a key-value pair.
 *
 * \param pair The pair to free
 * \param free_key How to free the key
 * \param free_value How to free the value
 *
 */
void kv_pair_data_free(kv_pair_t pair, prototype_t *key_proto,
					   prototype_t *value_proto);
DECLARE_PROTO(kv_pair_t);

#endif //!__KV_PAIR__H__