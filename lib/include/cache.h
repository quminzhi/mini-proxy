#pragma once
#ifndef CACHE_H
#define CACHE_H

#include <csapp.h>

#define MAX_CACHE_SIZE 1049000 /* 1 MiB */
#define MAX_OBJECT_SIZE 102400 /* 100 KiB */

/**********************************************
 * a cache queue based on doubly linked list
 *********************************************/

typedef struct cnode {
  // key: (host, port, path)
  char *host;
  char *port;
  char *path;
  // value: (payload: bytestream)
  char *payload;

  struct cnode *prev;
  struct cnode *next;
} cnode_t;

extern cnode_t *end;    /* end pointer */
extern cnode_t *front;  /* front pointer */
extern int cache_count; /* count of nodes except front and end dummy nodes */
extern volatile size_t cache_load; /* total cache load size */

extern volatile int readcnt; /* zero initially */
extern sem_t mutex;          /* protect readcnt */
extern sem_t w;              /* reader-favored style */

void cache_init();
void cache_destroy();

void enqueue(cnode_t *node);
cnode_t *dequeue();
void update(cnode_t *node);

cnode_t *is_cached(char *host, char *port, char *path);
int is_match(cnode_t *node, char *host, char *port, char *path);
int is_empty();

static inline void cache_check();
cnode_t *new_node(char *host, char *port, char *path, char *payload);
void free_node(cnode_t *node);

#endif /* CACHE_H */
