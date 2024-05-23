/**
 * Copyright (c) 2024
 * \file lmap.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Linked HashMap
 */

#ifndef __LMAP__H__
#define __LMAP__H__

#include "../impl/proto.h"
#include "../list/dlist.h"
#include "hmap.h"
#include "kv_pair.h"
/* An ordered HashMap. */
typedef struct {
	hmap_t access;
	dlist_t data;
	const prototype_t *value_proto;
} lmap_t;

/**
 * \brief Creates a new Linked Hashmap. For ease of use, please use `LMAP_NEW`.
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
lmap_t lm_new(const prototype_t *key_proto, const prototype_t *value_proto);

/**
 * \brief Sets `value` in the map at `key`.
 *
 * \param[in] lmap The Linked HashMap to set at
 * \param[in] key The key of the value
 * \param[in] value The value to set
 */
void lm_set(lmap_t *lmap, void *key, void *value);

/**
 * \brief Removes the value added in the map with `key`.
 *
 * \param[in] lmap The Linked HashMap to remove from
 * \param[in] key The key of the value
 */
void lm_remove(lmap_t *lmap, void *key);

/**
 * \brief Removes the first key-value pair added in the map.
 *
 * \param lmap The Linked HashMap to remove from
 */
void lm_remove_first(lmap_t *lmap);

/**
 * \brief Gets the value added in the map with `key`.
 *
 * \param[in] lmap The Linked HashMap to get from
 * \param[in] key The key of the value
 * \return The value stored at `key`. If no such value exists, returns `NULL`.
 */
void *lm_get(lmap_t *lmap, void *key);

/**
 * \brief Gets the first key-value pair added in the map.
 *
 * \param lmap The Linked HashMap to get from
 * \return The first key-value pair added in the map.
 */
kv_pair_t *lm_get_first(lmap_t *lmap);

/**
 * \brief Checks if a map contains `key`.
 *
 * \param lmap The Linked HashMap to check
 * \param key The key to check
 * \return `1` if the map contains the key. `0` otherwise.
 */
size_t lm_has(lmap_t *lmap, void *key);

/**
 * \brief Creates a new Linked HashMap.
 *
 * \param[in] key_type <type: Hash + Compare + Clone + Free> The type of the key
 * \param[in] value_type <type: Clone + Free> The type of the value
 *
 */
#define LMAP_NEW(key_type, value_type)                                         \
	lm_new(PROTOTYPE(key_type), PROTOTYPE(value_type))

/**
 * \brief Frees an Linked HashMap from memory.
 *
 * \param lmap The map to free.
 */
void lm_free(lmap_t *lmap);

DECLARE_PROTO(lmap_t);
#endif // !__LMAP__H__
