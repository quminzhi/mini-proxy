#pragma once
#ifndef PROXY_MODULE_H
#define PROXY_MODULE_H

#include <comm.h>
#include <csapp.h>
#include <cache.h>
#include <nio.h>

#define HEADER_LINE_SIZE 100 /* maximum lines read from request header */

/* define agent request header info */
extern char *user_agent_hdr;
extern char *accept_hdr;
extern char *accept_encoding_hdr;
extern char *connection_hdr;
extern char *proxy_connection_hdr;

void worker_main(int connfd);
int parse_request(int connfd, const char *request, char *host, char *port,
                  char *path);
int parse_uri(const char *uri, char *host, char *port, char *path);
void read_request_header(nio_t *nio_ptr);

int forward_request_response(int connfd, char *host, char *port, char *path);
void forward_request(int forwardfd, char *host, char *port, char *path);
size_t forward_response(int forwardfd, int connfd, char *payload);
size_t forward_response_cached(int connfd, char *payload_cache, size_t bytesize);

static inline void response_failure(int connfd, const char *cause,
                                    const char *errnum, const char *shortmsg,
                                    const char *longmsg);
#endif /* PROXY_MODULE_H */
