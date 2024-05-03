/**
 * \file vec.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-26
 * \brief Resizeable Vector
 */
#ifndef __VEC__H__
#define __VEC__H__
#include "err.h"
#include <stdlib.h>
#include "impl/iter.h"
/**
 * \brief A generic resizeable vector. `vec_t` is an Iterable object.
 */
typedef struct {
	void *data;
	size_t data_size;
	size_t len;
	size_t capacity;
} vec_t;

/**
 * \brief Creates a new vector instance that stores elements of size
 * `data_size`.
 *
 * \param[in] data_size The size of the elements in the vector
 *
 * \return A new vector instance.
 */
vec_t vec_new(size_t data_size);

/**
 * \brief Adds the value in `data` at the end of `vec`
 *
 * \param[in] vec The vector to add to
 * \param[in] data The data to add
 */
void vec_push(vec_t *vec, void *data);

/**
 * \brief Inserts the value in `data` in `vec` at position `index`.
 *
 * \param[in] vec The vector to add to
 * \param[in] index The position to add to
 * \param[in] data The data to add
 *
 * \note This moves all the elements after `index+1` so this operation is deemed
 * expensive.
 */
void vec_insert(vec_t *vec, size_t index, void *data);

/**
 * \brief Removes the element at position `index` from `vec`.
 *
 * \param[in] vec The vector to remove from
 * \param[in] index The position of the element to remove
 *
 * \note This moves all the elements after `index+1` so this operation is deemed
 * expensive.
 */
void vec_remove(vec_t *vec, size_t index);

/**
 * \brief Frees a vector from memory.
 *
 * \param[in] vec The vector to free.
 */
void vec_free(vec_t *vec);

/**
 * \brief Gets the address of the element at position `index` from `vec`.
 *
 * \param[in] vec [vec_t*] The vector get the element from
 * \param[in] index [size_t] The index of the element
 *
 * \return [void*] Address of the element at position `index`.
 */
#define VEC_ADR(vec, index)                                                    \
	({                                                                         \
		vec_t *__vci = (vec);                                                  \
		(char *)((__vci)->data) + (index) * (__vci)->data_size;                \
	})

/**
 * \brief Gets the value of the element at position `index` from `vec` as a
 * `type`. The position of the element is given by the `vec.data_size` property
 * and not `sizeof(type)`; It is just a reinterpretation.
 *
 * \param[in] vec [vec_t*] The vector get the element from
 * \param[in] type <type> The type to interpret the value as
 * \param[in] index [size_t] The index of the element
 *
 * \return [type] The value of the element at position `index`.
 *
 */
#define VEC_VAL(vec, type, index) (*((type *)(VEC_ADR(vec, index))))

/**
 * \brief Gets the index of the first element to meet the `criteria`.
 * The macro creates a variable visible only inside the expression named `el`
 * which represents the current value from the vector, which can be used in the
 * `criteria`.
 *
 * \param[in] vec_i    [vec_t*] The vector to look in.
 * \param[in] type     <type> The type of `el`.
 * \param[in] criteria (expr) The criteria to check against.
 *
 * \return [int] The index of the element. If no element is found, `-1` is
 * returned.
 */
#define VEC_INDEX_OF(vec_i, type, criteria)                                    \
	({                                                                         \
		int __ret = -1, __index = 0;                                           \
		for_iter(vec_t, __i, vec_i)                                            \
		{                                                                      \
			type el = ITER_VAL(__i, type);                                     \
			if (criteria) {                                                    \
				__ret = __index;                                               \
				break;                                                         \
			}                                                                  \
			__index++;                                                         \
		}                                                                      \
		__ret;                                                                 \
	})

/**
 * \brief Gets the index of the last element to meet the `criteria`.
 * The macro creates a variable visible only inside the expression named `el`
 * which represent the a value from the vector, which can be used in the
 * `criteria`.
 *
 * \param[in] vec_i    [vec_t*] The vector to look in.
 * \param[in] type     <type> The type of `el`.
 * \param[in] criteria (expr) The criteria to check against.
 *
 * \return [int] The index of the element. If no element is found, `-1` is
 * returned.
 */
#define VEC_INDEX_OF_REV(vec_i, type, criteria)                                \
	({                                                                         \
		vec_t *__vci = (vec_i);                                                \
		int __ret = -1, __index = __vci->len - 1;                              \
		for_iter_rev(vec_t, __i, __vci)                                        \
		{                                                                      \
			type el = ITER_VAL(__i, type);                                     \
			if (criteria) {                                                    \
				__ret = __index;                                               \
				break;                                                         \
			}                                                                  \
			__index--;                                                         \
		}                                                                      \
		__ret;                                                                 \
	})

/**
 * \brief Creates an iterator from a Vector.
 *
 * \param[in] list The list to iterate
 *
 * \return The created iterator.
 */
iter_t vec_t_iter_new(vec_t *list);

/**
 * \brief Creates a reverse iterator from a Vector.
 *
 * \param[in] list The list to iterate in reverse
 *
 * \return The created iterator.
 */
iter_t vec_t_iter_rev(vec_t *list);

/**
 * \brief Moves the iterator `iter` to the next value, if it iterates over a
 * Vector.
 *
 * \param[in] iter The iterator to move
 *
 */
void vec_t_iter_next(iter_t *iter);

/**
 * \brief Moves the iterator `iter` to the previous value, if it iterates over a
 * Vector.
 *
 * \param[in] iter The iterator to move
 *
 */
void vec_t_iter_prev(iter_t *iter);

#endif //!__VEC__H__
