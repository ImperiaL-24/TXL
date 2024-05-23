/**
 * \file proto.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-11
 * \brief Prototype
 */

#ifndef __PROTO__H__
#define __PROTO__H__

#include "cmp.h"
#include "hash.h"
#include "iter_type.h"
#include "stdlib.h"
/* How to implement the traits in a prototype */

#define __TYPE_IMPL_HASH(type) .hash = type##_hash,
#define __TYPE_IMPL_CLONE(type) .clone = type##_clone,
#define __TYPE_IMPL_FREE(type) .free = type##_free,
#define __TYPE_IMPL_FREE_DEFAULT(type) .free = NULL,
#define __TYPE_IMPL_CMP(type) .cmp = type##_cmp,

#define __TYPE_IMPL_ITER(type)                                                 \
	.iter_new = type##_iter_new, .iter_next = type##_iter_next,                \
	.iter_get = type##_iter_get,

#define __TYPE_IMPL_ITER_REV(type)                                             \
	.iter_rev = type##_iter_rev, .iter_prev = type##_iter_prev,                \
	.iter_get = type##_iter_get,

#define __TYPE_IMPL_ITER_FULL(type)                                            \
	.iter_new = type##_iter_new, .iter_next = type##_iter_next,                \
	.iter_rev = type##_iter_rev, .iter_prev = type##_iter_prev,                \
	.iter_get = type##_iter_get,

#define __TYPE_IMPL_SET(type)                                                  \
	.set_add = type##_set_add, .set_remove = type##_set_remove,                \
	.set_has = type##_set_has, __TYPE_IMPL_ITER(type)

/* Generic Trait Implementation */

#define IMPL(type, X) __TYPE_IMPL_##X(type)

/* Foreach macro hack */
#define FE_0(type)
#define FE_1(type, X) IMPL(type, X)
#define FE_2(type, X, ...) IMPL(type, X) FE_1(type, __VA_ARGS__)
#define FE_3(type, X, ...) IMPL(type, X) FE_2(type, __VA_ARGS__)
#define FE_4(type, X, ...) IMPL(type, X) FE_3(type, __VA_ARGS__)
#define FE_5(type, X, ...) IMPL(type, X) FE_4(type, __VA_ARGS__)

#define GET_MACRO(_0, _1, _2, _3, _4, _5, NAME, ...) NAME

/* Trait implementor */
#define IMPL_TRAITS(type, ...)                                                 \
	GET_MACRO(_0, __VA_ARGS__, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0)             \
	(type, __VA_ARGS__)

typedef struct {
	size_t size;
	hash_t (*hash)(void *);
	cmp_t (*cmp)(void *, void *);
	void (*free)(void *);
	void (*clone)(void *, void *);

	iter_t (*iter_new)(void *);
	iter_t (*iter_rev)(void *);
	void (*iter_next)(iter_t *);
	void (*iter_prev)(iter_t *);

	void *(*iter_get)(iter_t *);

	void (*set_add)(void *, void *);
	void (*set_remove)(void *, void *);
	size_t (*set_has)(void *, void *);

} prototype_t;

#define DEFINE_PROTO(type, ...)                                                \
	const prototype_t *__##type##_proto()                                      \
	{                                                                          \
		static const prototype_t proto = {                                     \
			IMPL_TRAITS(type, __VA_ARGS__).size = sizeof(type)};               \
		return &proto;                                                         \
	}

#define DECLARE_PROTO(type) const prototype_t *__##type##_proto()

#define PROTOTYPE(type) __##type##_proto()

#endif //!__PROTO__H__
