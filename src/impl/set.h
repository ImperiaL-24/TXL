/**
 * \file set.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-14
 * \brief Set Operations
 */

#ifndef __SET__H__
#define __SET__H__

#include "proto.h"

// TODO Set operations
void set_reunion(void *origin, void *other, const prototype_t *origin_proto,
				 const prototype_t *other_proto);

void set_intersect(void *origin, void *other, const prototype_t *origin_proto,
				   const prototype_t *other_proto);

void set_subtract(void *origin, void *other, const prototype_t *origin_proto,
				  const prototype_t *other_proto);

#endif //!__SET__H__