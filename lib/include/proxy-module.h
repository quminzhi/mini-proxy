#pragma once
#ifndef PROXY_MODULE_H
#define PROXY_MODULE_H

#include <comm.h>
#include <csapp.h>

#define MAX_CACHE_SIZE 104900
#define MAX_OBJECT_SIZE 102400 /* payload max size: 100KB */

/* define agent request header info */
const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";
const char *accept_hdr =
    "Accept: "
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
const char *connection_hdr = "Connection: close\r\n";
const char *proxy_connection_hdr = "Proxy-Connection: close\r\n\r\n";

int init_proxy(char *port);
void start_listen(int listenfd);
int parse_request(int connfd, const char *request, char *host,
                                char *port, char *path);
int parse_uri(const char *uri, char *host, char *port, char *path);

void solve(int connfd);
void read_request_header(rio_t *rio_ptr);
int forward_request_response(int connfd, char *host, char *port,
                                           char *path);
void response_failure(int connfd, const char *cause,
                                    const char *errnum, const char *shortmsg,
                                    const char *longmsg);
#endif /* PROXY_MODULE_H */
