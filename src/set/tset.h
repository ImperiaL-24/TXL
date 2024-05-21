/**
 * \file tset.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-21
 * \brief Tree Set
 */

#ifndef __TSET__H__
#define __TSET__H__

#include "../impl/proto.h"

typedef struct tsnode_t {
	void *data;
	int32_t priority;

	struct tsnode_t *left, *right, *parent;
} tsnode_t;

typedef struct {
	tsnode_t *root;
	const prototype_t *data_proto;
} tset_t;

tset_t ts_new(const prototype_t *data_proto);

void ts_add(void *tset, void *data);

void ts_remove(void *tset, void *data);

size_t ts_has(void *tset, void *data);

void ts_free(tset_t *tset);

DECLARE_PROTO(tset_t);

/**
 * \brief Creates a new Treeset that stores `type`
 * \param[in] type - <type: Free + Clone + Hash + Cmp>
 */
#define TSET_NEW(type) ts_new(PROTOTYPE(type))

DECLARE_PROTO(tset_t);

#endif //!__TSET__H__