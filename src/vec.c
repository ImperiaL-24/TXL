/**
 * \file vec.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-26
 * \brief Resizable Vector Implementation
 */
#include "vec.h"
#include "err.h"
#include <string.h>

/**
 * \brief Refits the vector `vec` to hold at least `new_size` elements. The
 * capacity of the vector may expand or shrink based on the current capacity
 * and the `new_size` parameter.
 *
 * The growth / shrink coefficient is 1.5.
 *
 * \param[in] vec Vector to resize
 * \param[in] new_size The size the vector should hold
 *
 * \note This function does not guarantee that `vec` will be resize to
 * `new_size`, it just guarantees that it can store `new_size` elements.
 */
static void __vec_resize(vec_t *vec, size_t new_size)
{
	/* If we need to grow */
	if (new_size > vec->capacity) {
		/* +1 to avoid getting locked in a 1 capacity vector */
		size_t next_size = (vec->capacity + 1) * 3 / 2;
		vec->data = realloc(vec->data, next_size * vec->data_size);
		vec->capacity = next_size;
		DIE(!vec->data, "Malloc Fail");

		/* If we need to shrink (but not too much) */
	} else if (vec->capacity * 2 / 3 >= new_size && vec->capacity > 1) {
		vec->data = realloc(vec->data, vec->capacity * 2 / 3 * vec->data_size);
		vec->capacity = vec->capacity * 2 / 3;
		DIE(!vec->data, "Malloc Fail");
	}
}

/**
 * \brief Unsafely sets an element in `vec` at position `index` to a value
 * stored at `data`. Value at `data` is copied into the vector.
 *
 * \param[in] vec The vector to add the data to
 * \param[in] index The position of where to store the data
 * \param[in] data The data to store
 *
 * \note This is for internal use only. Alternatives include `vec_push()` and
 * `vec_insert()`.
 */
static inline void __vec_unsafe_set(vec_t *vec, size_t index, void *data)
{
	memcpy(VEC_ADR(vec, index), data, vec->data_size);
}

vec_t vec_new(size_t data_size)
{
	vec_t vec = (vec_t){.data = malloc(1 * data_size),
						.capacity = 1,
						.len = 0,
						.data_size = data_size};
	DIE(!vec.data, "Malloc Fail");
	return vec;
}

void vec_push(vec_t *vec, void *data)
{
	__vec_resize(vec, vec->len + 1);
	__vec_unsafe_set(vec, vec->len, data);

	vec->len++;
}

void vec_insert(vec_t *vec, size_t index, void *data)
{
	__vec_resize(vec, vec->len + 1);
	memmove(VEC_ADR(vec, index + 1), VEC_ADR(vec, index),
			(vec->len - index) * vec->data_size);
	__vec_unsafe_set(vec, index, data);
	vec->len++;
}

void vec_remove(vec_t *vec, size_t index)
{
	memmove(VEC_ADR(vec, index), VEC_ADR(vec, index + 1),
			(vec->len - index - 1) * vec->data_size);
	__vec_resize(vec, --vec->len);
}

void vec_free(vec_t *vec) { free(vec->data); }

inline iter_t vec_t_iter_new(vec_t *vec)
{
	return (iter_t){.current = !vec->len ? NULL : vec->data, .iterable = vec};
}

inline iter_t vec_t_iter_rev(vec_t *vec)
{
	return (iter_t){.current = VEC_ADR(vec, vec->len == 0 ? 0 : vec->len - 1),
					.iterable = vec};
}

void vec_t_iter_next(iter_t *iter)
{
	/* The address of the next element */
	void *next_adr =
		((char *)iter->current) + ((vec_t *)iter->iterable)->data_size;

	/* If it goes out of the vector boundary then signal a stop */
	if ((char *)next_adr >=
		VEC_ADR(((vec_t *)iter->iterable), ((vec_t *)iter->iterable)->len)) {
		iter->current = NULL;
		return;
	}
	iter->current = next_adr;
}

void vec_t_iter_prev(iter_t *iter)
{
	/* The address of the previous element */
	void *prev_adr =
		((char *)iter->current) - ((vec_t *)iter->iterable)->data_size;

	/* If it goes out of the vector boundary then signal a stop */
	if (prev_adr < ((vec_t *)iter->iterable)->data) {
		iter->current = NULL;
		return;
	}
	iter->current = prev_adr;
}