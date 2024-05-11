/**
 * \file ref.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-11
 * \brief Reference
 */

#include "ref.h"
#include "clone.h"

impl_default_clone(ref_t);

DEFINE_PROTO(ref_t, FREE_DEFAULT, CLONE);