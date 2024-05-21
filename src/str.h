/**
 * Copyright (c) 2024
 * \file str.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief String
 */
#ifndef __STR__H__
#define __STR__H__

#include "impl/cmp.h"
#include "impl/hash.h"
#include "impl/proto.h"
#include <stdio.h>
#include <string.h>
#define MAX_FMT_LENGTH 4096
/* A C String. Implements Hash, Compare, Clone and Free */
typedef char *str_t;

/**
 * \brief Reads a string from the passed file.
 *
 * \param[in] file The file to read from.
 * \return The read string.
 */
str_t str_read(FILE *file);

/**
 * \brief Creates a `str_t` reference to a C String. Should be used when the C
 * String outlives the created str_t and when actions are read-only.
 *
 * \param cstring The C String to reference
 * \return The created string.
 */
str_t str_ref(str_t str);

/**
 * \brief Clones a string.
 *
 * \param str The string to clone
 * \return A different instance of the same string.
 */
str_t str_clone(str_t str);

/**
 * \brief Determines if a string represents a base 10 integer.
 *
 * \param[in] string The string to verify
 *
 * \return `1` if `string` represents an integer. `0` otherwise.
 */
size_t str_is_int(str_t string);

/**
 * \brief Frees a string from memory.
 *
 * \param str The string to free
 */
void str_free(void *str);

/* str_t Compare implementation */
uint8_t str_eq(str_t str1, str_t str2);

/**
 * \brief Creates a formatted string
 * \param[in] fmt [char*] The format
 * \param[in] ... [ ??? ] The parameters in the format
 */
#define STR_FMT(fmt, ...)                                                      \
	({                                                                         \
		str_t __ret;                                                           \
		char *__str = malloc(MAX_FMT_LENGTH * sizeof(char));                   \
		snprintf(__str, MAX_FMT_LENGTH, fmt, __VA_ARGS__);                     \
		__ret = str_from(__str);                                               \
		free(__str);                                                           \
		__ret;                                                                 \
	})

DECLARE_PROTO(str_t);

#endif // !__STR__H__
