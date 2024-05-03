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

typedef struct {
    hash_t (*hash_key)(void*);
    cmp_t (*cmp_key)(void*, void*);

    void (*free_key)(void*);
    void (*free_data)(void*);

    size_t data_size;
    size_t key_size;
    size_t length;
    size_t capacity;
    list_t* lists;
} hmap_t;

typedef struct {
    void* key;
    void* value;
} kv_pair_t;


hmap_t hm_new(hash_t (*hash_function)(void*),cmp_t (*cmp_function)(void*, void*),void (*free_key)(void*),void (*free_data)(void*), size_t key_size, size_t data_size);

void hm_set(hmap_t* hmap, void* key, void* value);

void hm_remove(hmap_t* hmap, void* key);

void *hm_get(hmap_t* hmap, void* key);

size_t hm_has(hmap_t* hmap, void* key);

#define HMAP_NEW(key_type, value_type) hm_new(key_type##_hash, key_type##_cmp, key_type##_free, value_type##_free, sizeof(key_type), sizeof(value_type));

#endif  //!__HMAP__H__
