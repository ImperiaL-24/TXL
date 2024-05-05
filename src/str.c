#include "str.h"
#include "err.h"

str_t str_from(char* cstring) {
    str_t string;
    string.length = strlen(cstring);
    string.data = malloc((string.length+1) * sizeof(char));
    DIE(!string.data, "Malloc Fail");
    memcpy(string.data, cstring, string.length + 1);
    return string;
}

str_t str_clone(str_t str) {
    return str_from(str.data);
}

hash_t str_t_hash(void *key)
{
    unsigned char *key_string = (unsigned char *) ((str_t*)key)->data;
    hash_t hash = 5381;
    int c;

    while ((c = *key_string++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

void str_t_clone(void* target, void* str) {
    str_t* new_str = (str_t*)target;
    str_t* origin_str = (str_t*)str;
    str_t created_str = str_from(origin_str->data);
    *new_str = created_str;
}

inline cmp_t str_t_cmp(void* str1, void* str2) {
    return strcmp(((str_t*)str1)->data, ((str_t*)str2)->data);
}

inline uint8_t str_eq(str_t str1, str_t str2) {
    return str_t_cmp(&str1, &str2) == 0;
}

inline void str_free(void* str) {
    free(((str_t*)str)->data);
}