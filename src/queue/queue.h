/**
 * Copyright (c) 2024
 * \file queue.h
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-07
 * \brief Queue
 */
#ifndef __QUEUE__H__
#define __QUEUE__H__

#include "../list/list.h"

#include "../impl/proto.h"

/* A queue. The queue owns the data and will free everything automatically. The
 * stored data must implement Free and Clone. */
typedef list_t queue_t;
/**
 * \brief Creates a new queue.
 *
 * \param data_size The size of the stored data
 * \param data_free How to free the data stored
 * \param data_clone How to clone the data
 * \return The created queue
 */
queue_t q_new(prototype_t *data_proto);

/**
 * \brief Adds an element at the end of a queue.
 *
 * \param queue The queue to add to
 * \param data  The data to add
 */
void q_add(queue_t *queue, void *data);

/**
 * \brief Removes the element at the start of a queue.
 *
 * \param queue The queue to remove from
 */
void q_pop(queue_t *queue);

/**
 * \brief Gets the front of a queue.
 *
 * \param queue The queue to get from
 * \return The data stored at the front.
 */
void *q_front(queue_t *queue);

/**
 * \brief Evaluates if `queue` has elements inside
 *
 * \param queue The queue to evaluate
 * \return `1` if the queue is empty, `0` otherwise.
 */
int q_is_empty(queue_t *queue);

/**
 * \brief Frees a queue and all the stored data inside.
 *
 * \param queue The queue to free.
 */
void q_free(queue_t *queue);

/**
 * \brief Creates a new queue
 *
 * \param[in] type <type: Free + Clone> The type to store in the queue
 */
#define QUEUE_NEW(type) q_new(PROTOTYPE(type))

#endif // !__QUEUE__H__
