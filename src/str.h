/**
 * \file str.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief String
 */
#ifndef __STR__H__
#define __STR__H__

#include "impl/hash.h"
#include "impl/cmp.h"
#include "vec.h"
#include <string.h>
typedef  struct {
    char* data;
    size_t length;
} str_t;

str_t str_ref_from(char* cstring);

str_t str_from(char* cstring);

str_t str_clone(str_t str);

hash_t str_t_hash(void *key);

cmp_t str_t_cmp(void* str1, void* str2);

uint8_t str_eq(str_t str1, str_t str2);

void str_t_clone(void* target, void* str);

void str_free(void* str);

#define str_t_free str_free

#endif  //!__STR__H__