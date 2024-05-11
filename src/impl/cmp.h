/**
 * Copyright (c) 2024
 * \file cmp.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Compare Trait
 */
#ifndef __CMP__H__
#define __CMP__H__

/* The return of a compare function */
typedef enum { EQUAL = 0, SMALLER = -1, BIGGER = 1 } cmp_t;

/* A compare function */
typedef cmp_t (*cmp_func_t)(void *, void *);

#endif  // !__CMP__H__
