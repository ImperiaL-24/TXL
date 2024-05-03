#include "int_impl.h"

hash_t uint32_t_hash(void *key)
{
    hash_t uint_key = *((uint32_t*)key);

    uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
    uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
    uint_key = (uint_key >> 16u) ^ uint_key;

    return uint_key;
}

cmp_t uint32_t_cmp(void *num1, void* num2) {
    int val1 = *(int*)num1;
    int val2 = *(int*)num2;
    if(val1 < val2) return LOWER;
    if(val1 == val2) return EQUAL;
    return BIGGER;
}
