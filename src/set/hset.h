/**
 * \file hset.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-14
 * \brief HashSet
 */

#ifndef __HSET__H__
#define __HSET__H__

#include "../impl/proto.h"
#include "../list/list.h"

typedef struct {
	const prototype_t *data_proto;
	size_t length;
	size_t capacity;
	list_t *lists;
} hset_t;

hset_t hs_new(const prototype_t *data_proto);

void hs_add(void *hset, void *data);

void hs_remove(void *hset, void *data);

size_t hs_has(void *hset, void *data);

void hs_free(hset_t *hset);

DECLARE_PROTO(hset_t);

/**
 * \brief Creates a new Hashset that stores `type`
 * \param[in] type - <type: Free + Clone + Hash + Cmp>
 */
#define HSET_NEW(type) hs_new(PROTOTYPE(type))
#endif //!__HSET__H__