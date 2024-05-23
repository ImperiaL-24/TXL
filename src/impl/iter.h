/**
 * \file iter.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-26
 * \brief Generic Iterator
 */

#ifndef __ITER__H__
#define __ITER__H__
#include "iter_type.h"
#include "proto.h"

/**
 * \brief An iterator wrapper. Also stores the prototype of the iterated object.
 *
 *
 */
typedef struct {
	iter_t iter;
	const prototype_t *itrable_proto;
} iter_wrapper_t;

/**
 * \brief Returns the current value at which `iter` is currently at.
 *
 * \param iter [iter_t] The iterator to access the current element from
 * \param type <type> The type of the element inside
 *
 * \return [type] The value of the current element.
 */
#define ITER_VAL(iter_name, type)                                              \
	(*(type *)_##iter_name.itrable_proto->iter_get(&_##iter_name.iter))

#define ITER_GET(iter_name)                                                    \
	(_##iter_name.itrable_proto->iter_get(&_##iter_name.iter))

#define for_iter_proto(proto, iter_name, itrable)                              \
	for (iter_wrapper_t _##iter_name = {.iter = proto->iter_new(itrable),      \
										.itrable_proto = proto};               \
		 _##iter_name.iter.current; proto->iter_next(&_##iter_name.iter))

#define for_iter_rev_proto(proto, iter_name, itrable)                          \
	for (iter_wrapper_t _##iter_name = {.iter = proto->iter_rev(itrable),      \
										.itrable_proto = proto};               \
		 _##iter_name.iter.current; proto->iter_prev(&_##iter_name.iter))

/**
 * \brief Iterates over a `itrable` of type `itype` from the start.
 *
 * \param itype   <type> The type of the iterable
 * \param iter    (name) The local name of the iterator.
 * This name should be used inside the for loop to access the iterator
 * \param itrable [type*] The iterable to start iterating from
 *
 * \attention An iterator for the specified type must be implemented. Check the
 * documentation for `iter_t`.
 */
#define for_iter(itype, iter, itrable)                                         \
	for_iter_proto(PROTOTYPE(itype), iter, itrable)

/**
 * \brief Iterates over a `itrable` of type `itype` from the end.
 *
 * \param itype   <type> The type of the iterable
 * \param iter    (name) The local name of the iterator.
 * This name should be used inside the for loop to access the iterator.
 * \param itrable [type*] The iterable to start iterating from.
 *
 * \attention An iterator for the specified type must be implemented. Check
 * the documentation for `iter_t`.
 */
#define for_iter_rev(itype, iter, itrable)                                     \
	for_iter_rev_proto(PROTOTYPE(itype), iter, itrable)

#endif //!__ITER__H__
