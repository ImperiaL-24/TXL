/**
 * \file hmap.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Unordered HashMap
 */
#ifndef __HMAP__H__
#define __HMAP__H__

#include "../impl/hash.h"
#include "../impl/cmp.h"
#include "../list/list.h"

/**
 * \brief An Unordered HashMap.
 * The hashmap owns it's data, making a deep copy of the data and its keys.
 */
typedef struct {
    hash_t (*hash_key)(void*);
    cmp_t (*cmp_key)(void*, void*);

    void (*free_key)(void*);
    void (*free_data)(void*);

    void (*clone_key)(void*,void*);
    void (*clone_data)(void*,void*);

    size_t data_size;
    size_t key_size;
    size_t length;
    size_t capacity;
    list_t* lists;
} hmap_t;

/**
 * \brief A key-value pair.
 */
typedef struct {
    void* key;
    void* value;
} kv_pair_t;

kv_pair_t kv_pair_new(void* key, void* value, void (*clone_key)(void*, void*), void (*clone_data)(void*, void*), size_t key_size, size_t data_size);

void kv_pair_free(kv_pair_t pair, void (*free_key)(void*), void (*free_value)(void*));


hmap_t hm_new(hash_t (*hash_function)(void*), cmp_t (*cmp_function)(void*, void*), void (*free_key)(void*),void (*free_data)(void*), void (*clone_key)(void*, void*), void (*clone_data)(void*, void*), size_t key_size, size_t data_size);

void hm_set(hmap_t* hmap, void* key, void* value);

void hm_remove(hmap_t* hmap, void* key);

void *hm_get(hmap_t* hmap, void* key);

size_t hm_has(hmap_t* hmap, void* key);

void hm_free(hmap_t* hmap);

#define HMAP_NEW(key_type, value_type) hm_new(key_type##_hash, key_type##_cmp, key_type##_free, value_type##_free, key_type##_clone, value_type##_clone, sizeof(key_type), sizeof(value_type))

#endif  //!__HMAP__H__
