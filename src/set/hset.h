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
	prototype_t *data_proto;
	size_t length;
	size_t capacity;
	list_t *lists;
} hset_t;

hset_t hs_new(prototype_t *data_proto);

void hs_add(hset_t *hset, void *data);

void hs_remove(hset_t *hset, void *data);

void hs_has(hset_t *hset, void *data);

DECLARE_PROTO(hset_t);

#endif //!__HSET__H__