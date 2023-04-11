#include <nio.h>

/*********************************************************************
 * The nio package - ming I/O functions
 **********************************************************************/

/*
 * nio_readn - read n bytes (unbuffered)
 */
ssize_t nio_readn(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) /* interrupted by sig handler return */
        nread = 0;        /* and call read() again */
      else if (errno == ECONNRESET) {
        fprintf(stderr, "ECONNRESET handled\n");
        return 0;
      } else
        return -1; /* errno set by read() */
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    bufp += nread;
  }
  return (n - nleft); /* return >= 0 */
}

/*
 * nio_writen -  write n bytes (unbuffered)
 */
ssize_t nio_writen(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) /* interrupted by sig handler return */
        nwritten = 0;     /* and call write() again */
      else if (errno == EPIPE) {
        fprintf(stderr, "EPIPE handled\n");
        return n;
      } else
        return -1; /* errorno set by write() */
    }
    nleft -= nwritten;
    bufp += nwritten;
  }
  return n;
}

/*
 * nio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, nio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    nio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, nio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t nio_read(nio_t *rp, char *usrbuf, size_t n) {
  int cnt;

  while (rp->nio_cnt <= 0) { /* refill if buf is empty */
    rp->nio_cnt = read(rp->nio_fd, rp->nio_buf, sizeof(rp->nio_buf));
    if (rp->nio_cnt < 0) {
      if (errno == ECONNRESET) {
        fprintf(stderr, "ECONNRESET handled\n");
        return 0;
      } else if (errno != EINTR) /* interrupted by sig handler return */
        return -1;
    } else if (rp->nio_cnt == 0) /* EOF */
      return 0;
    else
      rp->nio_bufptr = rp->nio_buf; /* reset buffer ptr */
  }

  /* Copy min(n, rp->nio_cnt) bytes from internal buf to user buf */
  cnt = n;
  if (rp->nio_cnt < n)
    cnt = rp->nio_cnt;
  memcpy(usrbuf, rp->nio_bufptr, cnt);
  rp->nio_bufptr += cnt;
  rp->nio_cnt -= cnt;
  return cnt;
}

/*
 * nio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void nio_readinitb(nio_t *rp, int fd) {
  rp->nio_fd = fd;
  rp->nio_cnt = 0;
  rp->nio_bufptr = rp->nio_buf;
}

/*
 * nio_readnb - Robustly read n bytes (buffered)
 */
ssize_t nio_readnb(nio_t *rp, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nread = nio_read(rp, bufp, nleft)) < 0) {
      if (errno == EINTR) /* interrupted by sig handler return */
        nread = 0;        /* call read() again */
      else
        return -1; /* errno set by read() */
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    bufp += nread;
  }
  return (n - nleft); /* return >= 0 */
}

/*
 * nio_readlineb - mingly read a text line (buffered)
 */
ssize_t nio_readlineb(nio_t *rp, void *usrbuf, size_t maxlen) {
  int n, rc;
  char c, *bufp = usrbuf;

  for (n = 1; n < maxlen; n++) {
    if ((rc = nio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      if (c == '\n')
        break;
    } else if (rc == 0) {
      if (n == 1)
        return 0; /* EOF, no data read */
      else
        break; /* EOF, some data was read */
    } else
      return -1; /* error */
  }
  *bufp = 0;
  return n;
}

/**********************************
 * Wrappers for ming I/O routines
 **********************************/
ssize_t Nio_readn(int fd, void *ptr, size_t nbytes) {
  ssize_t n;

  if ((n = nio_readn(fd, ptr, nbytes)) < 0)
    unix_error("nio_readn error");
  return n;
}

void Nio_writen(int fd, void *usrbuf, size_t n) {
  if (nio_writen(fd, usrbuf, n) != n)
    unix_error("nio_writen error");
}

void Nio_readinitb(nio_t *rp, int fd) { nio_readinitb(rp, fd); }

ssize_t Nio_readnb(nio_t *rp, void *usrbuf, size_t n) {
  ssize_t rc;

  if ((rc = nio_readnb(rp, usrbuf, n)) < 0)
    unix_error("nio_readnb error");
  return rc;
}

ssize_t Nio_readlineb(nio_t *rp, void *usrbuf, size_t maxlen) {
  ssize_t rc;

  if ((rc = nio_readlineb(rp, usrbuf, maxlen)) < 0) {
    printf("Fd = %d\n", rp->nio_fd);
    unix_error("nio_readlineb error");
  }
  return rc;
}
