/**
 * Copyright (c) 2024
 * \file str.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief String
 */
#include "str.h"
#include "err.h"
#include <stdlib.h>

str_t str_ref(str_t str)
{
	str_t string;
	string = str;
	return string;
}

str_t str_clone(str_t str)
{
	str_t string;
	size_t len = strlen(str);
	string = malloc((len + 1) * sizeof(char));
	DIE(!string, "Malloc Fail");
	memcpy(string, str, len + 1);
	return string;
}

hash_t str_t_hash(void *key)
{
	unsigned char *key_string = (unsigned char *)(*(str_t *)key);
	hash_t hash = 5381;
	int c;

	while ((c = *key_string++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

void str_t_clone(void *target, void *str)
{
	str_t *new_str = (str_t *)target;
	str_t *origin_str = (str_t *)str;
	*new_str = str_clone(*origin_str);
}

cmp_t str_t_cmp(void *str1, void *str2)
{
	return strcmp(*(str_t *)str1, *(str_t *)str2);
}

uint8_t str_eq(str_t str1, str_t str2) { return str_t_cmp(&str1, &str2) == 0; }

inline void str_free(void *str)
{
	if (!str)
		return;
	free(*(str_t *)str);
}
/* str_t Free implementation */
#define str_t_free str_free

DEFINE_PROTO(str_t, HASH, CLONE, CMP, FREE);