/**
 * Copyright (c) 2024
 * \file queue.c
 * \author Dobrescu Andrei-Traian - 315CA (andrei.dobrescu2402@stud.acs.upb.ro)
 * \date 2024-05-07
 * \brief Queue
 */

#include "queue.h"

inline queue_t q_new(prototype_t *data_proto) { return sll_new(data_proto); }

inline void q_add(queue_t *queue, void *data) { sll_addt(queue, data); }

inline void q_pop(queue_t *queue) { sll_remh(queue); }

inline void *q_front(queue_t *queue) { return queue->head->data; }

inline int q_is_empty(queue_t *queue) { return queue->size == 0; }

inline void q_free(queue_t *queue) { sll_free(queue); }
