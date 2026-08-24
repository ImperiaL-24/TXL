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
#define GET_MACRO_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, NAME, ...) NAME

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

#define __VT_TYPE(type) __vt_t_ ## type
#define VT_TYPE(type) __VT_TYPE(type)

#define __VT(type) __vt_ ## type
#define VT(type) __VT(type)

#define DEFINE_VT(T, ...) typedef struct __VA_ARGS__ VT_TYPE(T); const prototype_t *__##T##_proto(); extern const VT_TYPE(T) VT(T)
#define SET_VT(T, ...) const VT_TYPE(T) VT(T) = __VA_ARGS__

#define FUNC(name) .name = name
#define FUNC_AS(name, func) .name = func


#define __INFER_ENTRY(T) T : VT(T)

/* Foreach Infer */
#define FEI_0(type)
#define FEI_1(type) __INFER_ENTRY(type)
#define FEI_2(type, ...) __INFER_ENTRY(type), FEI_1(__VA_ARGS__)
#define FEI_3(type, ...) __INFER_ENTRY(type), FEI_2(__VA_ARGS__)
#define FEI_4(type, ...) __INFER_ENTRY(type), FEI_3(__VA_ARGS__)
#define FEI_5(type, ...) __INFER_ENTRY(type), FEI_4(__VA_ARGS__)
#define FEI_6(type, ...) __INFER_ENTRY(type), FEI_5(__VA_ARGS__)
#define FEI_7(type, ...) __INFER_ENTRY(type), FEI_6(__VA_ARGS__)
#define FEI_8(type, ...) __INFER_ENTRY(type), FEI_7(__VA_ARGS__)
#define FEI_9(type, ...) __INFER_ENTRY(type), FEI_8(__VA_ARGS__)
#define FEI_10(type, ...) __INFER_ENTRY(type), FEI_9(__VA_ARGS__)
#define FEI_11(type, ...) __INFER_ENTRY(type), FEI_10(__VA_ARGS__)

/* Infer functions */
#define INFER(T) _Generic((T), __TYPES)
#define INFER_STAR(T) _Generic(*(T), __TYPES)

/* Vtable access */
#define $static_get(var, func) (VT(var)).func

/* IFARGS Implementation */
#define GET(_0, _1) _0 
#define GET_(_0, _1) _1 

#define JOIN(_0, _1) _0 ## _1
#define EJOIN(_0, _1) JOIN(_0, _1)

#define FIRST(_, ...) _
#define EFIRST(_) FIRST(_)

#define REST(_0, ...) __VA_ARGS__

#define GET_GET(...) \
    EJOIN(GET, EFIRST(REST(,,##__VA_ARGS__ _)))

#define IFARGS(YES, NO, ...) GET_GET(__VA_ARGS__)(YES, NO)


/* Method name separation */
#define PREPEND_COMMA(...) , __VA_ARGS__
#define NO_COMMA()
#define PREPEND_COMMA_IF_NONEMPTY(...) IFARGS(PREPEND_COMMA, NO_COMMA, __VA_ARGS__)(__VA_ARGS__)

#define __METHOD_NAME(a, ...) a
#define __METHOD_ARGS(a, ...) __VA_ARGS__
#define METHOD_NAME(...) __METHOD_NAME __VA_ARGS__
#define METHOD_ARGS(...) __METHOD_ARGS __VA_ARGS__

/* $ implementation */
#define SELF_INSERT(self, name, ...) name(self PREPEND_COMMA_IF_NONEMPTY(__VA_ARGS__))

#define huh_star(var, func) (INFER_STAR(var)).SELF_INSERT(var, METHOD_NAME(func), METHOD_ARGS(func)) 

#define $simple(var, func) huh_star(var, METHOD$ ##func)

/* Infer macro for ease of use when defining __TYPES */
#define GET_TYPES(...)                                                 \
	GET_MACRO_12(_0, __VA_ARGS__, FEI_11,FEI_10,FEI_9,FEI_8,FEI_7,FEI_6,FEI_5,FEI_4,FEI_3,FEI_2,FEI_1,FEI_0)             \
	(__VA_ARGS__)

/* $u stacking */
#define Tu1(v1, v2) $static_get(v1, v2) 
#define Tu2(v1, v2, v3) $simple(Tu1(v1, v2), v3) 
#define Tu3(v1, v2, v3, v4) $simple(Tu2(v1,v2,v3), v4) 
#define Tu4(v1, v2, v3, v4, v5) $simple(Tu3(v1,v2,v3, v4), v5) 
#define Tu5(v1, v2, v3, v4, v5, v6) $simple(Tu4(v1,v2,v3,v4,v5),v6) 
#define $u(...) \
GET_MACRO(__VA_ARGS__, Tu5, Tu4, Tu3, Tu2, Tu1) \
(__VA_ARGS__)

/* $ stacking */
#define T1(v1, v2) $simple(v1, v2) 
#define T2(v1, v2, v3) $simple(T1(v1, v2), v3) 
#define T3(v1, v2, v3, v4) $simple(T2(v1,v2,v3), v4) 
#define T4(v1, v2, v3, v4, v5) $simple(T3(v1,v2,v3, v4), v5) 
#define T5(v1, v2, v3, v4, v5, v6) $simple(T4(v1,v2,v3,v4,v5),v6) 
#define $(...) \
GET_MACRO(__VA_ARGS__, T5, T4, T3, T2, T1) \
(__VA_ARGS__)

/* CLOJURE IMPLEMENTATION */

/* parameter list to type name converter */
#define UNDERSCORE(x) _ ## x

#define F_0(type)
#define F_1(type)UNDERSCORE(type)
#define F_2(type, ...) EJOIN(UNDERSCORE(type),F_1(__VA_ARGS__))
#define F_3(type, ...) EJOIN(UNDERSCORE(type),F_2(__VA_ARGS__))
#define F_4(type, ...) EJOIN(UNDERSCORE(type),F_3(__VA_ARGS__))
#define F_5(type, ...) EJOIN(UNDERSCORE(type),F_4(__VA_ARGS__))

#define GET_NAMED_PARAM(...) GET_MACRO(_0, __VA_ARGS__, F_5, F_4, F_3, F_2, F_1, F_0) \
    (__VA_ARGS__)

/* Splitters */
#define SPLITTER_PARAMS(x, y) x
#define DEPAREN(...) __VA_ARGS__
#define SPLITTER_PARAMS_DEPAREN(x, y) DEPAREN x
#define SPLITTER_RIGHT(x, y) y
#define DOLLAR_REM$
#define __SPLITTER_RETURN(a) DOLLAR_REM##a
#define SPLITTER_RETURN(a) __SPLITTER_RETURN(a)

#define SPLITTER(...) (__VA_ARGS__),
/* Type of a function */
#define FUNC_TYPE(a, b) EJOIN(EJOIN(functype_,SPLITTER_RETURN(a)),GET_NAMED_PARAM(b))

/* Using implementation */
#define _using(a) typedef SPLITTER_RETURN(SPLITTER_RIGHT(a)) (*FUNC_TYPE(SPLITTER_RIGHT(a), SPLITTER_PARAMS_DEPAREN(a))) SPLITTER_PARAMS(a)
#define using(func) _using(SPLITTER func)

/* $fn implementation */
#define fc(a) FUNC_TYPE(SPLITTER_RIGHT(a), SPLITTER_PARAMS_DEPAREN(a))
#define $fn(func) fc(SPLITTER func)


/* Clojure implementation */
#define _clojure_transform(a) SPLITTER_RETURN(SPLITTER_RIGHT(a)), SPLITTER_PARAMS(a)
#define clojure_transform(...) _clojure_transform(SPLITTER __VA_ARGS__)

#define call_clojure(...) clojure_transform __VA_ARGS__

#define _base(a) call_clojure(SPLITTER_PARAMS(a)) SPLITTER_RIGHT(a)
#define base(func) _base(SPLITTER func)

#define _clojure_base(ret, ...) ({ ret _anon __VA_ARGS__ ; _anon;})
#define clojure_base(a) _clojure_base(a)
#define $c(func) clojure_base(base(func))

#endif //!__PROTO__H__
