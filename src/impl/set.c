/**
 * \file set.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-20
 * \brief Set Operations
 */

#include "set.h"
#include "../vec.h"
#include "ref.h"

void set_reunion(void *origin, void *other, const prototype_t *origin_proto,
				 const prototype_t *other_proto)
{
	for_iter_proto(other_proto, i, other)
	{
		origin_proto->set_add(origin, other_proto->iter_get(&_i));
	}
}

void set_intersect(void *origin, void *other, const prototype_t *origin_proto,
				   const prototype_t *other_proto)
{
	// TODO: ref of elements might get freed from the set remove. A SEG FAULT
	// CAN COME FROM HERE IF THE TODO IN hset.c at line 63 IS NOT DEALT WITH
	vec_t vec = vec_new(PROTOTYPE(ref_t));

	/* vectorize the set */
	for_iter_proto(origin_proto, i, origin)
	{
		ref_t value_ref = origin_proto->iter_get(&_i);
		vec_push(&vec, &value_ref);
	}

	for_iter(vec_t, i, &vec)
	{
		if (!other_proto->set_has(other, ITER_VAL(i, ref_t))) {
			origin_proto->set_remove(origin, ITER_VAL(i, ref_t));
		}
	}

	vec_free(&vec);
}

void set_subtract(void *origin, void *other, const prototype_t *origin_proto,
				  const prototype_t *other_proto)
{
	for_iter_proto(other_proto, i, other)
	{
		origin_proto->set_remove(origin, other_proto->iter_get(&_i));
	}
}