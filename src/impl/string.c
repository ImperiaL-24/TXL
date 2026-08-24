#include "string.h"
#include "char_impl.h"
#include <stdarg.h>

static string_t new() {
    return vec_new(PROTOTYPE(char));
}

static string_t new_reserved(size_t size) {
    return vec_new_reserved(PROTOTYPE(char), size);
}

static string_t from_cnt(str_t str, size_t len) {
    if (len == 0) {
        return $u(string_t,new());
    }
    string_t s = $u(string_t,new_reserved(len));
    s.len = len;
    memcpy(s.data, str, s.len);
    return s;
}

static string_t from(str_t str) {
    return from_cnt(str, strlen(str));
}


static string_t fmt(str_t str, ...) {
    va_list args;
    va_start(args, str);

    va_list args2;
    va_copy(args2, args);

    int len = vsnprintf(NULL, 0, str, args);
    string_t s = $u(string_t,new_reserved(len+1));
    vsnprintf(s.data, len+1, str, args2);
    s.len = len;

    va_end(args2);
    va_end(args);
    return s;
}

static void reserve(string_t* self, size_t len) {
    vec_resize(self, self->len + len);
}

static void append_cnt(string_t *self, str_t str, size_t len) {
    if (len == 0) {
        return;
    }
    vec_resize(self, self->len + len);
    memcpy(VEC_ADR(self, self->len), str, len);
    self->len += len;
}

static void append(string_t *self, str_t str) {
    append_cnt(self, str, strlen(str));
}

static void append_char(string_t *self, char c) {
    vec_push(self, &c);
}

static void append_string(string_t *self, string_t str) {
    append_cnt(self, str.data, str.len);
}

static int equals(string_t *self, string_t other) {
    if (self->len != other.len) {
        return 0;
    }
    return memcmp(self->data, other.data, self->len) == 0;
}

static int starts_with(string_t *self, str_t other) {
    size_t slen = strlen(other);
    if (self->len < slen) {
        return 0;
    }
    return memcmp(self->data, other, slen) == 0;
}


static int equals_str(string_t *self, str_t other) {
    if (!other) {
        return 0;
    }
    size_t len = strlen(other);
    if (self->len != len) {
        return 0;
    }
    return memcmp(self->data, other, self->len) == 0;
}

static str_t to_str(string_t *self) {
    str_t ret = malloc(self->len + 1);
    memcpy(ret, self->data, self->len);
    ret[self->len] = '\0';
    return ret;
}

static void drop(string_t *self) {
    vec_free(self);
}

SET_VT(string_t, {
    FUNC(new),
    FUNC(new_reserved),
    FUNC(from),
    FUNC(from_cnt),
    FUNC(fmt),
    FUNC(reserve),
    FUNC(append),
    FUNC(append_cnt),
    FUNC(append_char),
    FUNC(append_string),
    FUNC(drop),
    FUNC(equals),
    FUNC(equals_str),
    FUNC(starts_with),
    FUNC(to_str),
});