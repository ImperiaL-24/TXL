#ifndef __STRING__H__
#define __STRING__H__

#include "cmp.h"
#include "hash.h"
#include "proto.h"
#include "str.h"
#include "../vec/vec.h"

typedef vec_t string_t;

#define STRING_FMT "%.*s"
#define STRING_PRNT(x) (int)(x).len, (char*)(x).data

DEFINE_VT(string_t, {
    string_t (*new)();
    string_t (*new_reserved)(size_t size);
    string_t (*from)(str_t str);
    string_t (*from_cnt)(str_t str, size_t size);
    string_t (*fmt)(str_t str, ...);
    void (*reserve)(string_t* self, size_t len);
    void (*append)(string_t *self, str_t str);
    void (*append_cnt)(string_t *self, str_t str, size_t len);
    void (*append_char)(string_t *self, char c);
    void (*append_string)(string_t *self, string_t str);
    int (*equals)(string_t *self, string_t other);
    int (*equals_str)(string_t *self, str_t other);
    int (*starts_with)(string_t *self, str_t other);
    str_t (*to_str)(string_t *self);
    void (*drop)(string_t *self);
});

#define METHOD$reserve(...) (reserve, __VA_ARGS__)
#define METHOD$append(...) (append, __VA_ARGS__)
#define METHOD$append_cnt(...) (append_cnt, __VA_ARGS__)
#define METHOD$append_char(...) (append_char, __VA_ARGS__)
#define METHOD$append_string(...) (append_string, __VA_ARGS__)
#define METHOD$drop(...) (drop, __VA_ARGS__)
#define METHOD$equals(...) (equals, __VA_ARGS__)
#define METHOD$equals_str(...) (equals_str, __VA_ARGS__)
#define METHOD$starts_with(...) (starts_with, __VA_ARGS__)
#define METHOD$to_str(...) (to_str, __VA_ARGS__)
#endif  //!__STRING__H__