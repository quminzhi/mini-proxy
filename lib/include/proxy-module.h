#pragma once
#ifndef PROXY_MODULE_H
#define PROXY_MODULE_H

#include <comm.h>
#include <csapp.h>

#define MAX_CACHE_SIZE 104900
#define MAX_OBJECT_SIZE 102400 /* payload max size: 100KB */

/* define agent request header info */
extern const char *user_agent_hdr;
extern const char *accept_hdr;
extern const char *accept_encoding_hdr;
extern const char *connection_hdr;
extern const char *proxy_connection_hdr;

void solve(int connfd);
int parse_request(int connfd, const char *request, char *host, char *port,
                  char *path);
int parse_uri(const char *uri, char *host, char *port, char *path);
void read_request_header(rio_t *rio_ptr);

int forward_request_response(int connfd, char *host, char *port, char *path);

static inline void response_failure(int connfd, const char *cause,
                                    const char *errnum, const char *shortmsg,
                                    const char *longmsg);
#endif /* PROXY_MODULE_H */
