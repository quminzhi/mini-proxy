#include <sbuf.h>

void sbuf_setup(sbuf_ptr_t sp, int n) {
  sp->buf = calloc(n, sizeof(int));
  sp->n = n;
  // (front, rear]
  sp->front = sp->rear = 0;
  sem_init(&sp->mutex, 0, 1); /* lock */
  sem_init(&sp->slots, 0, n);
  sem_init(&sp->items, 0, 0);
}

void subf_teardown(sbuf_ptr_t sp) {
  free(sp->buf);
}

void sbuf_insert(sbuf_ptr_t sp, int item) {
  P(&sp->slots);
  P(&sp->mutex);
  sp->buf[(++sp->rear) % sp->n] = item;
  V(&sp->mutex);
  V(&sp->items);
}

// return removed item
int sbuf_remove(sbuf_ptr_t sp) {
  int removed;
  P(&sp->items);
  P(&sp->mutex);
  removed = sp->buf[(++sp->front) % sp->n];
  V(&sp->mutex);
  V(&sp->slots);

  return removed;
}

int sbuf_is_empty(sbuf_ptr_t sp) {
  return sp->front == sp->rear;
}
