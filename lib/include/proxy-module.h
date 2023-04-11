#pragma once
#ifndef PROXY_MODULE_H
#define PROXY_MODULE_H

#include <comm.h>
#include <csapp.h>
#include <cache.h>

#define HEADER_LINE_SIZE 100 /* maximum lines read from request header */

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
void forward_request(int forwardfd, char *host, char *port, char *path);
size_t forward_response(int forwardfd, int connfd, char *payload);
size_t forward_response_cached(int connfd, cnode_t *node);

static inline void response_failure(int connfd, const char *cause,
                                    const char *errnum, const char *shortmsg,
                                    const char *longmsg);
#endif /* PROXY_MODULE_H */
