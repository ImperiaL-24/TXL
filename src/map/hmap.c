#include "hmap.h"
#include "../err.h"
#include "../impl/iter.h"

int __hm_resize(hmap_t* hmap, size_t new_size) {
    /* If we need to grow */
	if (new_size > hmap->capacity) {
		/* +1 to avoid getting locked in a 1 capacity hmap */
		size_t next_size = (hmap->capacity + 1) * 3 / 2;
		hmap->lists = realloc(hmap->lists, next_size * hmap->data_size);
		hmap->capacity = next_size;
		DIE(!hmap->lists, "Malloc Fail");
        // TODO: rehashing
		
        return 1;
	}
    /* If we need to shrink (but not too much) */
    if (hmap->capacity * 2 / 3 >= new_size && hmap->capacity > 10) {
		hmap->lists = realloc(hmap->lists, hmap->capacity * 2 / 3 * hmap->data_size);
		hmap->capacity = hmap->capacity * 2 / 3;
        // TODO: rehashing
		DIE(!hmap->lists, "Malloc Fail");
        return 1;
	}

    return 0;
}


hmap_t hm_new(hash_t (*hash_function)(void*),cmp_t (*cmp_function)(void*, void*),void (*free_key)(void*),void (*free_data)(void*), size_t key_size, size_t data_size) {
    #define INITIAL_CAPACITY 10
    hmap_t hmap;
    hmap.capacity = INITIAL_CAPACITY;

    hmap.hash_key = hash_function;
    hmap.cmp_key = cmp_function;
    hmap.free_key = free_key;
    hmap.free_data = free_data;

    hmap.length = 0;
    hmap.key_size = key_size;
    hmap.data_size = data_size;
    hmap.lists = malloc(sizeof(list_t) * INITIAL_CAPACITY);
    DIE(!hmap.lists, "Malloc Fail");
    for(int i = 0; i < INITIAL_CAPACITY; i++) {
        hmap.lists[i] = LIST_NEW(sizeof(kv_pair_t));
    }
    return hmap;
    #undef INITIAL_CAPACITY
}


void hm_set(hmap_t* hmap, void* key, void* value) {
    hash_t hash = hmap->hash_key(key);
    size_t idx = hash % hmap->capacity;
    // printf("ADDING TO IDX: %d\n", idx);
    list_t* list = hmap->lists + idx;
    for_iter(list_t, i, list) {
        if(hmap->cmp_key(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
            if(hmap->free_data)
                hmap->free_data(SLITER_VAL(i, kv_pair_t).value);
            void* new_data = malloc(hmap->data_size);
            memcpy(new_data, value, hmap->data_size);
            SLITER_VAL(i, kv_pair_t).value = new_data;

            return;
        }
    }
    hmap->length++;
    if(__hm_resize(hmap, hmap->length)) {
        // TODO: rehash to get the new list
    }

    void* new_data = malloc(hmap->data_size);
    memcpy(new_data, value, hmap->data_size);
    void* new_key = malloc(hmap->key_size);
    memcpy(new_key, key, hmap->key_size);
    kv_pair_t pair = (kv_pair_t) {.key = new_key, .value = new_data};
    sll_addh(list, &pair);
    // printf("DEBUG -> ADDED TO LIST\n");
     
}

void hm_remove(hmap_t* hmap, void* key) {
    hash_t hash = hmap->hash_key(key);
    size_t idx = hash % hmap->capacity;
    lnode_t* prev = NULL;
    for_iter(list_t, i, hmap->lists + idx) {
        if(hmap->cmp_key(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
            lnode_t* prev_ref = prev ? prev : hmap->lists[idx].head;
            sll_rem_after(hmap->lists + idx, prev_ref);
            hmap->length--;
            __hm_resize(hmap, hmap->length);
            return;
        }
        prev = &ITER_VAL(i, lnode_t);
    }
    
}

void *hm_get(hmap_t* hmap, void* key) {
    hash_t hash = hmap->hash_key(key);
    size_t idx = hash % hmap->capacity;
    for_iter(list_t, i, hmap->lists + idx) {
        if(hmap->cmp_key(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
            return SLITER_VAL(i, kv_pair_t).value;
        }
    }
    return NULL;
}

size_t hm_has(hmap_t* hmap, void* key) {
    hash_t hash = hmap->hash_key(key);
    size_t idx = hash % hmap->capacity;
    for_iter(list_t, i, hmap->lists + idx) {
        if(hmap->cmp_key(SLITER_VAL(i, kv_pair_t).key, key) == 0) {
            return 1;
        }
    }
    return 0;
}