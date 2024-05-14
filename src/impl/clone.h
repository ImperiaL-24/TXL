/**
 * Copyright (c) 2024
 * \file clone.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-05
 * \brief Clone Trait
 */
#ifndef __CLONE__H__
#define __CLONE__H__

/**
 * \brief Creates a Shallow Copy Clone Implementation for the specified type.
 *
 * \param[in] type <type> The type to create the implementation for.
 *
 */
#define impl_default_clone(type)                                               \
	void type##_clone(void *target, void *origin)                              \
	{                                                                          \
		*(type *)target = *(type *)origin;                                     \
	}

#endif // !__CLONE__H__
