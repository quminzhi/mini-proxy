#include <assert.h>
#include <cache.h>

cnode_t *end;    /* end pointer */
cnode_t *front;  /* front pointer */
int cache_count; /* count of nodes except front and end dummy nodes */
volatile size_t cache_load; /* total cache load size */

volatile int readcnt; /* zero initially */
sem_t mutex;          /* protect readcnt */
sem_t w;              /* reader-favored style */

/*!
 * @brief cache_init initialize cache queue
 *
 * cache queue: dummy -> node 1 -> node 2 -> ... -> node n -> dummy
 *              front    LRU  --------------------- MRU       end
 *
 * LRU: least recently used, MRU: most recently used
 */
void cache_init() {
  cache_load = 0;
  cache_count = 0;
  readcnt = 0;
  sem_init(&mutex, 0, 1);
  sem_init(&w, 0, 1);

  front = new_node("", "", "", "");
  end = new_node("", "", "", "");
  front->next = end, front->prev = NULL;
  end->prev = front, end->next = NULL;
}

/*!
 * @brief new_node creates a new node and allocates memory in heap for each
 * field
 */
cnode_t *new_node(char *host, char *port, char *path, char *payload) {
  cnode_t *res = (cnode_t *)malloc(sizeof(cnode_t));

  res->host = (char *)malloc(strlen(host) + 1);
  strcpy(res->host, host);
  res->port = (char *)malloc(strlen(port) + 1);
  strcpy(res->port, port);
  res->path = (char *)malloc(strlen(path) + 1);
  strcpy(res->path, path);
  res->payload = (char *)malloc(strlen(payload) + 1);
  strcpy(res->payload, payload);

  return res;
}

/*!
 * @brief free_node frees dynamic memory allocated when the node is created
 */
void free_node(cnode_t *node) {
  free(node->host);
  free(node->port);
  free(node->path);
  free(node->payload);
  free(node);
}

/*!
 * @brief cache_destroy destroys the cache queue
 */
void cache_destroy() {
  cnode_t *nxt;
  for (cnode_t *p = front; p != NULL;) {
    nxt = p->next;
    free_node(p);
    p = nxt;
  }

  cache_load = 0;
  cache_count = 0;
}

/*!
 * @brief enqueue append a node to the end of the queue
 *
 * node must be a valid cache node.
 */
void enqueue(cnode_t *node) {
  node->prev = end->prev;
  node->next = end;
  end->prev = node;
  node->prev->next = node;

  cache_count++;
  cache_load += strlen(node->payload);

  // remove LRU nodes if cache load overflows
  cnode_t *del;
  while (cache_load > MAX_CACHE_SIZE) {
    del = dequeue();
    free_node(del);
  }
}

/*!
 * @brief dequeue delete a node from the front
 *
 * INFO: Note reources of deleted node are not freed
 *
 * return a pointer to that node.
 */
cnode_t *dequeue() {
  if (is_empty())
    return NULL;
  cnode_t *del = front->next;
  front->next = del->next;
  front->next->prev = front;

  cache_count--;
  cache_load -= strlen(del->payload);

  return del;
}

/*!
 * @brief update updates the "timestamp" of node, that is move it to the end
 *
 * note that cache count and cache payload remain unchanged during update
 * operation.
 */
void update(cnode_t *node) {
  cnode_t *tomove = node;
  tomove->prev->next = tomove->next;
  tomove->next->prev = tomove->prev;

  tomove->next = end;
  tomove->prev = end->prev;
  end->prev = tomove;
  tomove->prev->next = tomove;
}

/*!
 * @brief is_empty checks if queue is empty
 *
 * return 1 if empty, 0 if not empty
 */
int is_empty() { return front->next == end && front == end->prev; }

/*!
 * @brief is_cached checks if node with key (host, port, path) is cached
 *
 * return the cached node if success, NULL otherwise
 */
cnode_t *is_cached(char *host, char *port, char *path) {
  if (is_empty())
    return NULL;
  cnode_t *p;
  for (p = end->prev; p != front; p = p->prev) {
    if (is_match(p, host, port, path)) {
      return p;
    }
  }
  return NULL;
}

/*!
 * @brief node_cmp compare node with its key (host, port, path)
 *
 * return 1 if match, 0 if not match
 */
int is_match(cnode_t *node, char *host, char *port, char *path) {
  if (strcasecmp(node->host, host) != 0)
    return 0;
  if (strcasecmp(node->port, host) != 0)
    return 0;
  if (strcasecmp(node->path, host) != 0)
    return 0;
  return 1;
}

/*!
 * @brief cache_check checks the status of cache queue
 *
 * when cache count = 0, 1, and more, if cache queue is correct
 */
static inline void cache_check() {
  if (cache_count == 0) {
    assert(is_empty() == 1);
    assert(front->prev == NULL && end->next == NULL);
  } else {
    size_t count = 0;
    for (cnode_t *p = front->next; p != end; p = p->next) {
      count++;
      assert(p == p->prev->next);
    }
    assert(count == cache_count);
  }
}
