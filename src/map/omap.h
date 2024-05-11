/**
 * Copyright (c) 2024
 * \file omap.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Ordered HashMap
 */

#ifndef __OMAP__H__
#define __OMAP__H__

#include "../impl/proto.h"
#include "../list/dlist.h"
#include "hmap.h"
#include "kv_pair.h"
/* An ordered HashMap. */
typedef struct {
	hmap_t access;
	dlist_t data;
	prototype_t *value_proto;
	// void (*free_data)(void *);
	// void (*clone_data)(void *, void *);
	// size_t data_size;
} omap_t;

/**
 * \brief Creates a new Ordered Hashmap. For ease of use, please use `OMAP_NEW`.
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
omap_t om_new(prototype_t *key_proto, prototype_t *value_proto);

/**
 * \brief Sets `value` in the map at `key`.
 *
 * \param[in] omap The Ordered HashMap to set at
 * \param[in] key The key of the value
 * \param[in] value The value to set
 */
void om_set(omap_t *omap, void *key, void *value);

/**
 * \brief Removes the value added in the map with `key`.
 *
 * \param[in] omap The Ordered HashMap to remove from
 * \param[in] key The key of the value
 */
void om_remove(omap_t *omap, void *key);

/**
 * \brief Removes the first key-value pair added in the map.
 *
 * \param omap The Ordered HashMap to remove from
 */
void om_remove_first(omap_t *omap);

/**
 * \brief Gets the value added in the map with `key`.
 *
 * \param[in] omap The Ordered HashMap to get from
 * \param[in] key The key of the value
 * \return The value stored at `key`. If no such value exists, returns `NULL`.
 */
void *om_get(omap_t *omap, void *key);

/**
 * \brief Gets the first key-value pair added in the map.
 *
 * \param omap The Ordered HashMap to get from
 * \return The first key-value pair added in the map.
 */
kv_pair_t *om_get_first(omap_t *omap);

/**
 * \brief Checks if a map contains `key`.
 *
 * \param omap The Ordered HashMap to check
 * \param key The key to check
 * \return `1` if the map contains the key. `0` otherwise.
 */
size_t om_has(omap_t *omap, void *key);

/**
 * \brief Creates a new Ordered HashMap.
 *
 * \param[in] key_type <type: Hash + Compare + Clone + Free> The type of the key
 * \param[in] value_type <type: Clone + Free> The type of the value
 *
 */
#define OMAP_NEW(key_type, value_type)                                         \
	om_new(PROTOTYPE(key_type), PROTOTYPE(value_type))

/**
 * \brief Creates an iterator from an Ordered HashMap.
 *
 * \param[in] list The map to iterate
 *
 * \return The created iterator.
 */
iter_t omap_t_iter_new(omap_t *omap);

/**
 * \brief Creates a reverse iterator from an Ordered HashMap.
 *
 * \param[in] list The map to iterate in reverse
 *
 * \return The created iterator.
 */
iter_t omap_t_iter_rev(omap_t *omap);

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over
 * an Ordered HashMap.
 *
 * \param[in] iter The iterator to move
 *
 */
void omap_t_iter_next(iter_t *iter);

/**
 * \brief Moves the iterator `iter` to the previous value, if it iterates over
 * an Ordered HashMap.
 *
 * \param[in] iter The iterator to move
 *
 */
void omap_t_iter_prev(iter_t *iter);

/**
 * \brief Frees an Ordered HashMap from memory.
 *
 * \param omap The map to free.
 */
void om_free(omap_t *omap);

/**
 * \brief Gets the current value at which `iter` is at, as a `type`, if the
 * iterable is a Ordered HashMap. Since an iterator over a `omap_t` iterates
 * over nodes, using the normal methods would get the node at which is at and
 * not the value itself.
 *
 * \param[in] iter [iter_t] The iterator to get the value from
 * \param[in] type <type> The type of the stored value
 *
 * \return [type] The current value of the iterator.
 */
#define OMITER_VAL(iter, type) (*(type *)DLITER_VAL(iter, kv_pair_t).value)

/**
 * \brief Gets the current key at which `iter` is at, as a `type`, if the
 * iterable is a Ordered HashMap. Since an iterator over a `omap_t` iterates
 * over nodes, using the normal methods would get the node at which is at and
 * not the key itself.
 *
 * \param[in] iter [iter_t] The iterator to get the key from
 * \param[in] type <type> The type of the stored key
 *
 * \return [type] The current key of the iterator.
 */
#define OMITER_KEY(iter, type) (*(type *)DLITER_VAL(iter, kv_pair_t).key)

#endif // !__OMAP__H__
