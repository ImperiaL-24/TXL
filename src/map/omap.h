/**
 * \file omap.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-02
 * \brief Ordered HashMap
 */

#include "hmap.h"
#include "../list/dlist.h"

typedef struct {
    hmap_t access;
    dlist_t data;
    void (*free_data)(void*);
    void (*clone_data)(void*,void*);
    size_t data_size;
} omap_t;

omap_t om_new(hash_t (*hash_function)(void*), cmp_t (*cmp_function)(void*, void*), void (*free_key)(void*),void (*free_data)(void*), void (*clone_key)(void*,void*), void (*clone_data)(void*,void*), size_t key_size, size_t data_size);

void om_set(omap_t* omap, void* key, void* value);

void om_conset(omap_t* omap, void* key, void* value);

void om_remove(omap_t* omap, void* key);

void om_remove_first(omap_t* omap);

void *om_get(omap_t* omap, void* key);

size_t om_has(omap_t* omap, void* key);

#define OMAP_NEW(key_type, value_type) om_new(key_type##_hash, key_type##_cmp, key_type##_free, value_type##_free, key_type##_clone, value_type##_clone, sizeof(key_type), sizeof(value_type));

/**
 * \brief Creates an iterator from an Ordered HashMap.
 *
 * \param[in] list The map to iterate
 *
 * \return The created iterator.
 */
iter_t omap_t_iter_new(omap_t* omap);

/**
 * \brief Creates a reverse iterator from an Ordered HashMap.
 *
 * \param[in] list The map to iterate in reverse
 *
 * \return The created iterator.
 */
iter_t omap_t_iter_rev(omap_t* omap);

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

void om_free(omap_t* omap);

#define OMITER_VAL(iter, type) (*(type*)DLITER_VAL(iter, kv_pair_t).value)

#define OMITER_KEY(iter, type) (*(type*)DLITER_VAL(iter, kv_pair_t).key)