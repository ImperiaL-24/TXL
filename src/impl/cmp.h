/**
 * \file cmp.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-01
 * \brief Compare Return Value
 */
#ifndef __CMP__H__
#define __CMP__H__


typedef enum {
    EQUAL = 0,
    LOWER = -1,
    BIGGER = 1
} cmp_t;

typedef cmp_t (*cmp_func_t)(void*, void*);

#endif  //!__CMP__H__