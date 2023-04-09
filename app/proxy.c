#include "proxy-module.h"
#include <comm.h>
#include <csapp.h>
#include <inet-helper.h>
#include <stdio.h>

/*! TODO: Proxy server
 *
 * @todo a basic sequential proxy that handles HTTP/1.0 GET requests
 */

void sigpipe_handler(int sig) {
  LOG("SIGPIPE handled\n");
  return;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  signal(SIGPIPE, sigpipe_handler);

  int listenfd = init_proxy(argv[1]);
  start_listen(listenfd);

  return 0;
}
