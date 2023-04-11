#pragma once
#ifndef NIO_H
#define NIO_H

#include "csapp.h"

#define NIO_BUFSIZE 8192
typedef struct {
  int nio_fd;                /* descriptor for this internal buf */
  int nio_cnt;               /* unread bytes in internal buf */
  char *nio_bufptr;          /* next unread byte in internal buf */
  char nio_buf[NIO_BUFSIZE]; /* internal buffer */
} nio_t;

ssize_t nio_readn(int fd, void *usrbuf, size_t n);
ssize_t nio_writen(int fd, void *usrbuf, size_t n);
void nio_readinitb(nio_t *rp, int fd);
ssize_t nio_readnb(nio_t *rp, void *usrbuf, size_t n);
ssize_t nio_readlineb(nio_t *rp, void *usrbuf, size_t maxlen);

ssize_t Nio_readn(int fd, void *usrbuf, size_t n);
void Nio_writen(int fd, void *usrbuf, size_t n);
void Nio_readinitb(nio_t *rp, int fd);
ssize_t Nio_readnb(nio_t *rp, void *usrbuf, size_t n);
ssize_t Nio_readlineb(nio_t *rp, void *usrbuf, size_t maxlen);

#endif /* end of include guard: NIO_H */
