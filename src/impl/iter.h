/**
 * \file iter.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-26
 * \brief Generic Iterator
 */

#ifndef __ITER__H__
#define __ITER__H__
#include <stdlib.h>

/**
 * \brief A generic iterator. Used to iterate over objects that can be iterable;
 *
 * This acts more like an interface for other iterators. In other words,
 * it needs to be implemented.
 *
 * Function implementations require to have the name of the type of the iterable
 * in front of each function.
 *
 * Example:
 * The iterable object is `dlist_t`
 * The iter_t function implementations require `dlist_t_` in front of each
 * function
 *
 *
 * An iterator implementation requires the following functions to be
 * implemented:
 *      - `iter_new(iterable*)` -> Used to create a iter_t from the iterable
 * object. By convention, `iter_new` starts from the beginning of the iterable.
 *
 *      - `iter_rev(iterable*)` -> Used to create a iter_t from the iterable
 * object By convention, `iter_rev` starts from the end of the iterable.
 *
 *      - `iter_next(iter_t*)` -> Used to move to the next element in the
 * iterable `iter_next` must set the `iter_t.current` property properly. If the
 * iterable has no next element, `iter_t.current` must be set to `NULL`.
 *
 *      - `iter_prev(iter_t*)` -> Used to move to the previous element in the
 * iterable `iter_prev` must set the `iter_t.current` property properly. If the
 * iterable has no previous element, `iter_t.current` must be set to `NULL`.
 *
 * If all the methods are implemented properly then the `for_iter` and the
 * `for_iter_rev` macros can be used to iterate easily over the object.
 *
 *
 * `ITER_VAL` macro is used to get a pointer to the current element in the list.
 * Iterator implementation may create additional macros for directly getting the
 * data inside.
 *
 *
 */
typedef struct {
	void *current;
	void *iterable;
	void *next;
	size_t metadata;
} iter_t;

/**
 * \brief Returns the current value at which `iter` is currently at.
 *
 * \param iter [iter_t] The iterator to access the current element from
 * \param type <type> The type of the element inside
 *
 * \return [type] The value of the current element.
 */
#define ITER_VAL(iter, type) (*(type *)_##iter.current)

#define for_iter_proto(proto, iter, itrable)                                   \
	for (iter_t _##iter = proto->iter_new(itrable); _##iter.current;           \
		 proto->iter_next(&_##iter))

#define for_iter_rev_proto(proto, iter, itrable)                               \
	for (iter_t _##iter = proto->iter_rev(itrable); _##iter.current;           \
		 proto->iter_prev(&_##iter))

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
