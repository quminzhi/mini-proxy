#pragma once
#ifndef SBUF_H
#define SBUF_H

#include <semaphore.h>
#include <csapp.h>

typedef struct {
  int *buf;  /* bounded array buf */
  int n;  /* maximum number of slots */
  int front; /* buf[(front + 1) % n] is the first item */
  int rear; /* buf[rear % n] is the last item */
  sem_t mutex;  /* protects access to the buffer */
  sem_t slots; /* counts available slots */
  sem_t items; /* counts available items */
} sbuf_t, *sbuf_ptr_t;

void sbuf_setup(sbuf_ptr_t sp, int n);
void subf_teardown(sbuf_ptr_t sp);
void sbuf_insert(sbuf_ptr_t sp, int item);
// return removed item
int sbuf_remove(sbuf_ptr_t sp);

#endif  /* SBUF_H */
