#include "proxy.h"
#include <comm.h>
#include <csapp.h>
#include <inet-helper.h>
#include <proxy-module.h>
#include <stdio.h>

/*! TODO: Proxy server
 *
 * @todo a basic sequential proxy that handles HTTP/1.0 GET requests
 */

/*!
 * @brief lisen_on initializes listen fd listening on port and spawning
 * connection fd for a connection.
 */
void listen_on(int listenfd) {
  int connfd, rc;
  struct sockaddr_storage clientaddr;
  socklen_t clientlen = sizeof(struct sockaddr_storage);
  char clientname[MAXLINE];
  char client_port[MAXLINE];

  pthread_t tid;

  while (1) {
    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    sbuf_insert(&sbuf, connfd);

    if ((rc = getnameinfo((struct sockaddr *)&clientaddr, clientlen, clientname,
                          MAXLINE, client_port, MAXLINE, 0) != 0)) {
      fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(rc));
      exit(1);
    }

    INFO("Connected to (%s, %s) and associated it with connfd(%d)\n",
         clientname, client_port, connfd);
  }
}

void worker_setup() {
  pthread_t tid;
  for (int i = 0; i < NTHREADS; i++) {
    // pass value not pointer for 4th parameter
    pthread_create(&tid, NULL, worker, (void *)(uint64_t)i);
  }
}

/*!
 * @brief worker fetches request from buffer queue and make a request/response
 * transaction
 *
 * to avoid race condition, buffer queue is a concurrency-safe producer-consumer
 * model
 */
void *worker(void *vargp) {
  // detach worker thread
  pthread_detach(pthread_self());

  uint64_t wid = (uint64_t)vargp;

  INFO("Worker[%lu] is created and ready for service\n", wid);

  while (1) {
    // try to retrieve connfd from the buffer queue
    int connfd = sbuf_remove(&sbuf);
    LOG("Worker[%lu] retrieved connfd(%d)\n", wid, connfd);
    // service
    solve(connfd);
    close(connfd);
    LOG("Worker[%lu] has released connfd(%d)\n", wid, connfd);
  }

  return NULL;
}

void sigpipe_handler(int sig) {
  INFO("SIGPIPE received: pipe was broken\n");
  return;
}

void sigterm_handler(int sig) {
  cache_destroy();
  INFO("Cache destroyed, exit by SIGTERM\n");
  return;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  signal(SIGPIPE, sigpipe_handler);
  signal(SIGTERM, sigterm_handler);

  cache_init();
  sbuf_setup(&sbuf, SBUFSIZE);
  worker_setup();

  int listenfd = open_listenfd(argv[1]);
  INFO("Listenfd has been initialized successfully\n");
  listen_on(listenfd);

  // destroy cache to prevent memory leakage
  cache_destroy();

  return 0;
}
