#include "str.h"
#include "err.h"

str_t str_from(char* cstring) {
    str_t string;
    string.length = strlen(cstring);
    string.data = malloc(string.length * sizeof(char));
    DIE(!string.data, "Malloc Fail");
    memcpy(string.data, cstring, string.length);
    return string;
}

str_t str_clone(str_t str) {
    str_t string;
    string.length = str.length;
    string.data = malloc(string.length * sizeof(char));
    DIE(!string.data, "Malloc Fail");
    memcpy(string.data, str.data, string.length);
    return string;
}

hash_t str_t_hash(void *key)
{
    unsigned char *key_string = (unsigned char *) key;
    hash_t hash = 5381;
    int c;

    while ((c = *key_string++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

inline cmp_t str_t_cmp(void* str1, void* str2) {
    return strcmp(((str_t*)str1)->data, ((str_t*)str2)->data);
}

inline uint8_t str_eq(str_t str1, str_t str2) {
    return str_t_cmp(&str1, &str2) == 0;
}

inline void str_t_free(str_t str) {
    free(str.data);
}