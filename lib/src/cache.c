#include <cache.h>

cnode_t *tail;   /* tail pointer */
cnode_t *front;  /* front pointer */
int cache_count; /* count of nodes except front and end dummy nodes */
volatile size_t cache_load; /* total cache load size */

volatile int readcnt; /* zero initially */
sem_t mutex;          /* protect readcnt */
sem_t w;              /* reader-favored style */

/*!
 * @brief cache_init initialize cache queue
 *
 * two dummy nodes are created to prevent edge cases
 */
void cache_init() {
  cache_load = 0;
  cache_count = 0;
  readcnt = 0;
  sem_init(&mutex, 0, 1);
  sem_init(&w, 0, 1);

  front = new_node("", "", "", "", -1);
  tail = new_node("", "", "", "", -1);
  front->next = tail;
  tail->prev = front;
}

void cache_destroy(cnode_t *node);
void enqueue(cnode_t *node);
cnode_t *dequeue();

cnode_t *new_node(char *host, char *port, char *path, char *payload,
                  size_t size) {
  cnode_t *res = (cnode_t *)malloc(sizeof(cnode_t));

  res->host = (char *)malloc(strlen(host) + 1);
  strcpy(res->host, host);
  res->port = (char *)malloc(strlen(port) + 1);
  strcpy(res->port, port);
  res->path = (char *)malloc(strlen(path) + 1);
  strcpy(res->path, path);
  res->payload = (char *)malloc(strlen(payload) + 1);
  strcpy(res->payload, payload);
  res->size = size;

  return res;
}

/*!
 * @brief is_empty checks if queue is empty
 *
 * return 1 if empty, 0 if not empty
 */
int is_empty() { return front->next == tail && front == tail->prev; }

/*!
 * @brief is_cached checks if node with key (host, port, path) is cached
 *
 * return the cached node if success, NULL otherwise
 */
cnode_t *is_cached(char *host, char *port, char *path) {
  if (is_empty())
    return NULL;
  cnode_t *res;
  for (res = tail->prev; res != front; res = res->prev) {
    if (is_match(res, host, port, path)) {
      return res;
    }
  }
  return NULL;
}

/*!
 * @brief node_cmp compare node with its key (host, port, path)
 *
 * return 1 if match, 0 if not match
 */
int is_match(cnode_t *this, char *host, char *port, char *path) {
  if (strcasecmp(this->host, host) != 0)
    return 0;
  if (strcasecmp(this->port, host) != 0)
    return 0;
  if (strcasecmp(this->path, host) != 0)
    return 0;
  return 1;
}

int cache_check();
