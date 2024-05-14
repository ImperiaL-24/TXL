/**
 * Copyright (c) 2024
 * \file hmap.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Unordered HashMap
 */
#ifndef __HMAP__H__
#define __HMAP__H__

#include "../impl/cmp.h"
#include "../impl/hash.h"
#include "../impl/proto.h"
#include "../list/list.h"
/**
 * \brief An Unordered HashMap.
 * The hashmap owns it's data, making a deep copy of the data and its keys.
 */
typedef struct {
	const prototype_t *key_proto;
	const prototype_t *value_proto;
	size_t length;
	size_t capacity;
	list_t *lists;
} hmap_t;

/**
 * \brief Creates a new Unordered Hashmap. For ease of use, please use
 * `HMAP_NEW`.
 *
 * \param hash_function How to hash a key
 * \param cmp_function How to compare 2 keys
 * \param free_key How to free a key
 * \param free_data How to free the data
 * \param clone_key How to clone a key
 * \param clone_data How to clone data
 * \param key_size The size of the key
 * \param data_size The size of the data
 * \return The created map.
 */
hmap_t hm_new(const prototype_t *key_proto, const prototype_t *value_proto);

/**
 * \brief Sets `value` in the map at `key`.
 *
 * \param[in] hmap The Unordered HashMap to set at
 * \param[in] key The key of the value
 * \param[in] value The value to set
 */
void hm_set(hmap_t *hmap, void *key, void *value);

/**
 * \brief Removes the value added in the map with `key`.
 *
 * \param[in] hmap The Unordered HashMap to remove from
 * \param[in] key The key of the value
 */
void hm_remove(hmap_t *hmap, void *key);

/**
 * \brief Gets the value added in the map with `key`.
 *
 * \param[in] hmap The Unordered HashMap to get from
 * \param[in] key The key of the value
 * \return The value stored at `key`. If no such value exists, returns `NULL`.
 */
void *hm_get(hmap_t *hmap, void *key);

/**
 * \brief Checks if a map contains `key`.
 *
 * \param hmap The Unordered HashMap to check
 * \param key The key to check
 * \return `1` if the map contains the key. `0` otherwise.
 */
size_t hm_has(hmap_t *hmap, void *key);

/**
 * \brief Frees an Unordered HashMap from memory.
 *
 * \param hmap The map to free.
 */
void hm_free(hmap_t *hmap);

/**
 * \brief Creates a new Unordered HashMap.
 *
 * \param[in] key_type <type: Hash + Compare + Clone + Free> The type of the key
 * \param[in] value_type <type: Clone + Free> The type of the value
 *
 */
#define HMAP_NEW(key_type, value_type)                                         \
	hm_new(PROTOTYPE(key_type), PROTOTYPE(value_type))
#endif // !__HMAP__H__
