#pragma once
#ifndef PROXY_H
#define PROXY_H

#include <sbuf.h>

#define NTHREADS 8
#define SBUFSIZE 16

sbuf_t sbuf;

/* MUST: define agent request header info which is referenced by proxy-module */
const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";
const char *accept_hdr =
    "Accept: "
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
const char *connection_hdr = "Connection: close\r\n";
const char *proxy_connection_hdr = "Proxy-Connection: close\r\n\r\n";

void listen_on(int listenfd);
void worker_setup();

void sigpipe_handler(int sig);

// create a thread to handle a request (connfd)
void *worker(void *vargp);

#endif /* PROXY_H */
