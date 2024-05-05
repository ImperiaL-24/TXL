
/**
 * \file int_impl.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Integer Default Implementations
 */
#ifndef __INT_IMPL__H__
#define __INT_IMPL__H__
#include "cmp.h"
#include "hash.h"
#include <stddef.h>

hash_t uint32_t_hash(void *key);

cmp_t uint32_t_cmp(void *num1, void* num2);

void uint32_t_clone(void* target, void* origin);

#define uint32_t_free NULL
#endif  //!__INT_IMPL__H__