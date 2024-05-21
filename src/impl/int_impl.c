/**
 * Copyright (c) 2024
 * \file int_impl.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Integer Default Implementations
 */
#include "int_impl.h"
#include "clone.h"
#include "cmp.h"
#include "hash.h"
#include <stddef.h>
/* uint32_t Hash implementation */
static hash_t uint32_t_hash(void *key)
{
	hash_t uint_key = *((uint32_t *)key);

	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = (uint_key >> 16u) ^ uint_key;

	return uint_key;
}
/* uint32_t Compare implementation */
static cmp_t uint32_t_cmp(void *num1, void *num2)
{
	int val1 = *(int *)num1;
	int val2 = *(int *)num2;
	if (val1 < val2)
		return SMALLER;
	if (val1 == val2)
		return EQUAL;
	return BIGGER;
}

/* uint32_t Clone implementation */
impl_default_clone(uint32_t);

DEFINE_PROTO(uint32_t, CLONE, HASH, CMP, FREE_DEFAULT);