/**
 * \file err.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-03-25
 * \brief Error Handling
 */
#ifndef __ERR__H__
#define __ERR__H__
#include <errno.h>
#include <stdio.h>

/**
 * \brief Asserts `assertion` at runtime, and exits the program if it is false.
 * printing `call_description`.
 *
 * \param[in] assertion (expr) The assertion to evaluate.
 * \param[in] call_description [char*] The message to display.
 */
#define DIE(assertion, call_description)                                       \
	do {                                                                       \
		if (assertion) {                                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
			perror(call_description);                                          \
			exit(errno);                                                       \
		}                                                                      \
	} while (0)

#endif //!__ERR__H__
