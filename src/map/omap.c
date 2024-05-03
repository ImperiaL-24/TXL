#include "omap.h"

omap_t om_new(hash_t (*hash_function)(void*),cmp_t (*cmp_function)(void*, void*),void (*free_key)(void*),void (*free_data)(void*), size_t key_size, size_t data_size) {
    omap_t map;
    map.access = hm_new(hash_function, cmp_function, free_key, free_data, key_size, sizeof(dlnode_t*));
    map.data = DLIST_NEW(data_size);
    return map; 
}

void om_set(omap_t* omap, void* key, void* value) {
    //TODO: if there is already data there then replace it plzthx
    dll_addt(&omap->data, value);
    hm_set(&omap->access, key, &omap->data.tail);
}

void om_conset(omap_t* omap, void* key, void* value) {
    if(!omap->data.size) {
        om_set(omap, key, value);
    }
    void* data = hm_get(&omap->access, key);
    if(data) {
        memmove(data, value, omap->data.data_size);
        return;
    }
    hash_t hash = omap->access.hash_key(key);
    for_iter(dlist_t, i, &omap->data) {
        hash_t data_hash = omap->access.hash_key(key);
        if(hash < data_hash) continue;
        dll_add_after(&omap->data, &ITER_VAL(i, dlnode_t), value);
        hm_set(&omap->access, key, &ITER_VAL(i, dlnode_t).next);
        return;
    }
    
}

void om_remove(omap_t* omap, void* key) {
    dll_rem_node(&omap->data, hm_get(&omap->access,key));
    hm_remove(&omap->access, key);
}

void om_remove_first(omap_t* omap) {
    if(!omap->data.size) return;
    kv_pair_t pair = *(kv_pair_t*)omap->data.head->data;
    hm_remove(&omap->access, pair.key);
    dll_remh(&omap->data);
}

inline void *om_get(omap_t* omap, void* key) {
    dlnode_t* node = *(dlnode_t**)hm_get(&omap->access, key);

    return node ? node->data : NULL;
}

inline size_t om_has(omap_t* omap, void* key) {
    return hm_has(&omap->access, key);
}

inline iter_t omap_t_iter_new(omap_t* omap) {
    return dlist_t_iter_new(&omap->data);
}

inline iter_t omap_t_iter_rev(omap_t* omap) {
    return dlist_t_iter_rev(&omap->data);
}

inline void omap_t_iter_next(iter_t *iter){
    dlist_t_iter_next(iter);
}

inline void omap_t_iter_prev(iter_t *iter){ 
    dlist_t_iter_prev(iter);
}