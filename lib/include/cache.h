#pragma once
#ifndef CACHE_H
#define CACHE_H

#include <csapp.h>

#define MAX_CACHE_SIZE 1049000 /* 1 MiB */
#define MAX_OBJECT_SIZE 102400 /* 100 KiB */

/* reader-writer model: reader favored style
 * 
 * writers must have exclusive access to the object, but readers may share the
 * object with an unlimited number of other readers
 */
extern volatile int readcnt; /* multi-thread: prevent readcnt from being cached in any
                         register */
extern sem_t mutex;          /* reader lock */
extern sem_t w;              /* writer lock */

static inline void pre_read();
static inline void post_read();
static inline void pre_write();
static inline void post_write();

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

void cache_init();
void add_cache(char *host, char *port, char *path, char *payload);
void cache_destroy();

cnode_t *new_node(char *host, char *port, char *path, char *payload);
void free_node(cnode_t *node);

void enqueue(cnode_t *node);
cnode_t *dequeue();
void update(cnode_t *node);

char *is_cached(char *host, char *port, char *path);
int is_empty();

static inline void cache_check();
static inline int is_match(cnode_t *node, char *host, char *port, char *path);

#endif /* CACHE_H */
