#include "omap.h"

// hv_pair_t hv_pair_new(hash_t hash, void* value, size_t value_size) {
//     void* new_value = malloc(value_size);
//     memcpy(new_value, value, value_size);
//     return (hv_pair_t) {.hash = hash, .value = new_value};
// }

// void hv_pair_free(hv_pair_t pair, void (*free_value)(void*)) {
//     if(free_value)
//         free_value(pair.value);
//     free(pair.value);
// }

static inline void __om_replace(omap_t* omap, dlnode_t**node_ref, kv_pair_t pair, void* key) {
    kv_pair_t* value_loc = (kv_pair_t*)(*node_ref)->data;
    kv_pair_free(*value_loc,omap->access.free_key, omap->free_data);
    memmove(value_loc, &pair, sizeof(kv_pair_t));
    dlnode_t* node = *node_ref;
    hm_remove(&omap->access, key);

    hm_set(&omap->access, key, &node);
}

void __node_clone(void* area, void* origin) {
    *(dlnode_t**)area = *(dlnode_t**)origin;

}

omap_t om_new(hash_t (*hash_function)(void*), cmp_t (*cmp_function)(void*, void*), void (*free_key)(void*),void (*free_data)(void*), void (*clone_key)(void*,void*), void (*clone_data)(void*,void*), size_t key_size, size_t data_size) {
    omap_t map;
    map.access = hm_new(hash_function, cmp_function, free_key, NULL, clone_key, __node_clone, key_size, sizeof(dlnode_t*));
    map.data = DLIST_NEW(sizeof(kv_pair_t));
    map.free_data = free_data;
    map.data_size = data_size;
    map.clone_data = clone_data;
    return map; 
}

void om_set(omap_t* omap, void* key, void* value) {
    dlnode_t** node_ref = hm_get(&omap->access, key);
    kv_pair_t pair = kv_pair_new(key, value, omap->access.clone_key, omap->clone_data, omap->access.key_size, omap->data_size);
    if(!node_ref) {
        
        dll_addt(&omap->data, &pair);
        hm_set(&omap->access, key, &omap->data.tail);
        return;
    }
    __om_replace(omap, node_ref, pair, key);
    
}

void om_conset(omap_t* omap, void* key, void* value) {
    if(!omap->data.size) {
        om_set(omap, key, value);
    }
    kv_pair_t pair = kv_pair_new(key, value, omap->access.clone_key, omap->clone_data, omap->access.key_size, omap->data_size);
    dlnode_t** node_ref = hm_get(&omap->access, key);
    if(!node_ref) {
        hash_t hash = omap->access.hash_key(key);
        for_iter_rev(dlist_t, i, &omap->data) {
            hash_t data_hash = omap->access.hash_key(DLITER_VAL(i, kv_pair_t).key);
            if(data_hash > hash) continue;
            dll_add_after(&omap->data, &ITER_VAL(i, dlnode_t), &pair);
            hm_set(&omap->access, key, &ITER_VAL(i, dlnode_t).next);
            return;
        }
        return;
    }
    __om_replace(omap, node_ref, pair, key);
}

void om_remove(omap_t* omap, void* key) {
    dlnode_t** node = hm_get(&omap->access,key);
    if(!node) return;

    kv_pair_t* value_loc = (kv_pair_t*)(*node)->data;
    kv_pair_free(*value_loc,omap->access.free_key, omap->free_data);
    
    dll_rem_node(&omap->data, *node);
    hm_remove(&omap->access, key);
}

void om_remove_first(omap_t* omap) {
    if(!omap->data.size) return;
    kv_pair_t pair = *(kv_pair_t*)omap->data.head->data;
    dll_remh(&omap->data);
    hm_remove(&omap->access, pair.key);
    kv_pair_free(pair, omap->access.free_key, omap->free_data);
}

inline void *om_get(omap_t* omap, void* key) {
    dlnode_t** node_ref = hm_get(&omap->access, key);

    return node_ref ? ((kv_pair_t*)(*node_ref)->data)->value : NULL;
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

void om_free(omap_t* omap) {
    hm_free(&omap->access);
    for_iter(dlist_t, i, &omap->data) {
        kv_pair_free(DLITER_VAL(i, kv_pair_t), omap->access.free_key, omap->free_data);
    }

    dll_free(&omap->data);

}