#include <comm.h>
#include <csapp.h>
#include <inet-helper.h>
#include <proxy-module.h>
#include <stdio.h>

/*!
 * @brief init_proxy: initialize listen file descriptor on given port
 *
 * bind and listen a file descriptor on port
 */
int init_proxy(char *port) {
  int listenfd = open_listenfd(port);
  INFO("Listenfd has been initialized successfully\n");
  return listenfd;
}

/*!
 * @brief start_lisen initializes listen fd listening on port and spawning
 * connection fd for a connection.
 */
void start_listen(int listenfd) {
  int connfd, rc;
  struct sockaddr_storage clientaddr;
  socklen_t clientlen = sizeof(struct sockaddr_storage);
  char client_uriname[MAXLINE];
  char client_port[MAXLINE];

  while (1) {
    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    if ((rc = getnameinfo((struct sockaddr *)&clientaddr, clientlen,
                          client_uriname, MAXLINE, client_port, MAXLINE,
                          0) != 0)) {
      fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(rc));
      exit(1);
    }

    INFO("Connected to (%s, %s)\n", client_uriname, client_port);

    // start service for client associated with connfd
    solve(connfd);
    close(connfd);
  }
}

/*!
 * @brief parse_request parses the request, validates it and give back host,
 * port, and path.
 *
 * return true if parse success, false otherwise
 */
int parse_request(int connfd, const char *request, char *host, char *port,
                  char *path) {
  char method[MAXLINE];   /* RESTFUL operation */
  char uri[MAXLINE];      /* save uri */
  char protocol[MAXLINE]; /* save protocol version */

  memset(method, 0, sizeof method);
  memset(uri, 0, sizeof uri);
  memset(protocol, 0, sizeof protocol);

  // empty request
  if (strcmp(request, "") == 0) {
    response_failure(connfd, request, "400", "Bad request",
                     "Valid request format: [method] [uri] [version], ex> "
                     "GET http://google.com HTTP/1.0");
    return 0;
  }

  sscanf(request, "%s %s %s", method, uri, protocol);
  LOG("solve: method = %s, uri = %s, protocol = %s\n", method, uri, protocol);

  if (strlen(method) == 0 || strlen(uri) == 0 || strlen(protocol) == 0) {
    response_failure(connfd, request, "400", "Bad request",
                     "Valid request format: [method] [uri] [version], ex> "
                     "GET http://google.com HTTP/1.0");
    return 0;
  }

  if (strcasecmp(method, "GET") != 0) {
    response_failure(connfd, method, "501", "Not implemented",
                     "Mini Proxy does not implement this method yet");
    return 0;
  }

  if (strcasecmp(protocol, "HTTP/1.0") != 0 &&
      strcasecmp(protocol, "HTTP/1.1") != 0) {
    response_failure(connfd, protocol, "400", "Bad request",
                     "Mini Proxy requires request version to be either "
                     "HTTP/1.0 or HTPP/1.1");
    return 0;
  }

  if (!parse_uri(uri, host, port, path)) {
    response_failure(connfd, uri, "404", "Not found",
                     "Mini Proxy couldn't parse the request");
    return 0;
  }

  LOG("parse_request: host = %s, port = %s, path = %s\n", host, port, path);

  return 1;
}

/*!
 * @brief forward_response forwards response from destination server to the
 * client
 *
 * return total response in bytes
 */
int forward_request_response(int connfd, char *host, char *port, char *path) {
  char payload[MAX_OBJECT_SIZE]; /* cache for respond info */

  rio_t forward_rio;
  int forwardfd = open_clientfd(host, port);
  rio_readinitb(&forward_rio, forwardfd);

  char forwardbuf[MAXLINE]; /* buffer for forward request */
  // forward request
  sprintf(forwardbuf, "GET %s HTTP/1.0\r\n", path);
  rio_writen(forwardfd, forwardbuf, strlen(forwardbuf));
  sprintf(forwardbuf, "Host: %s\r\n", host);
  rio_writen(forwardfd, forwardbuf, strlen(forwardbuf));
  rio_writen(forwardfd, user_agent_hdr, strlen(user_agent_hdr));
  rio_writen(forwardfd, accept_hdr, strlen(accept_hdr));
  rio_writen(forwardfd, accept_encoding_hdr, strlen(accept_encoding_hdr));
  rio_writen(forwardfd, connection_hdr, strlen(connection_hdr));
  rio_writen(forwardfd, proxy_connection_hdr, strlen(proxy_connection_hdr));

  // forward response
  size_t n;
  int sum = 0; /* sum of forward respond size in byte */
  memset(payload, 0, sizeof payload);
  while ((n = rio_readlineb(&forward_rio, forwardbuf, MAXLINE)) != 0) {
    sum += n;
    if (sum <= MAX_OBJECT_SIZE) {
      strcat(payload, forwardbuf);
      rio_writen(connfd, forwardbuf, n);
    }
  }
  return sum;
}

/*!
 * @brief solve: handle one HTTP request/response transaction
 *
 *  following things will be done for making a transaction:
 *    - read the entirety of request
 *    - parse line and validate request (HTTP/1.0 GET) format
 *    - establish connection from proxy to destination server
 */
void solve(int connfd) {
  char request[MAXLINE]; /* save request */
  char host[MAXLINE];    /* save host */
  char path[MAXLINE];    /* save path */
  char port[MAXLINE];    /* port in string format */

  memset(host, 0, sizeof host);
  memset(path, 0, sizeof path);
  memset(port, 0, sizeof port);

  // associate rio with connfd
  rio_t rio;
  rio_readinitb(&rio, connfd);

  size_t n;
  if ((n = rio_readlineb(&rio, request, MAXLINE)) != 0) {
    LOG("Proxy received %d bytes from connfd(%d)\n", (int)n, connfd);
    if (!parse_request(connfd, request, host, port, path)) {
      // if parse failed, return
      return;
    }
    read_request_header(&rio);
    int sum = forward_request_response(connfd, host, port, path);
    LOG("Responded %d bytes for connfd(%d)\n", sum, connfd);
  }
}

/*!
 * @brief read_request_header reads and parses HTTP request headers
 *
 * note that request header ends with a blank line ("\r\n")
 */
void read_request_header(rio_t *rio_ptr) {
  char buf[MAXLINE];
  rio_readlineb(rio_ptr, buf, MAXLINE);
  LOG("%s", buf);
  while (strcmp(buf, "\r\n") != 0) {
    rio_readlineb(rio_ptr, buf, MAXLINE);
    LOG("%s", buf);
  }
  return;
}

/*!
 * @brief parse_uri parses uri into host, port, path. For simplicity, path
 * includes query string in our program
 *
 *  uri format: [hostname](:port)[path](?query_string)
 *
 * return true if success, false otherwise
 */
int parse_uri(const char *uri, char *host, char *port, char *path) {
  // INFO: read scheme
  char scheme[10];
  char port_s[10];
  char *p = NULL;
  if ((p = strchr(uri, ':')) == NULL) {
    // no scheme specified (http, https)
    return 0;
  }

  int len = p - uri;
  strncpy(scheme, uri, len);
  scheme[len] = '\0';
  if (strcasecmp(scheme, "http") != 0) {
    LOG("Requested uri includes unsupported scheme, only support http request "
        "so far");
    return 0;
  }

  // INFO: read host
  p++;
  // check and skip '//'
  for (int i = 0; i < 2; i++) {
    if (p[i] != '/') {
      return 0;
    }
  }
  p += 2;

  char *q = p;
  while (q && *q != '\0' && *q != '/' && *q != ':' && *q != '\n')
    q++;
  len = q - p;
  strncpy(host, p, q - p);
  host[len] = '\0';

  // INFO: read port
  p = q;
  if (q && *q == ':') {
    // port is specified
    p = q + 1;
    q = p;
    while (q && *q != '\0' && isdigit(*q))
      q++;
    len = q - p;
    strncpy(port, p, len);
    p = q;
  } else {
    // set default port
    strcpy(port, "80");
  }

  // INFO: extract path
  if (p && *p == '/') {
    q = p;
    while (q && *q != '\0' && *q != '\n')
      q++;
    len = q - p;
    strncpy(path, p, len);
    path[len] = '\0';
  } else {
    // set default path if no path is specified
    strcpy(path, "/");
  }

  return 1;
}

/*!
 * @brief res_failure returns an error message to the client
 */
void response_failure(int connfd, const char *cause, const char *errnum,
                      const char *shortmsg, const char *longmsg) {
  char buf[MAXLINE], body[MAXBUF];

  // build the HTTP response body
  sprintf(body, "<html><title>Proxy Error</title>");
  sprintf(body,
          "%s<body bgcolor="
          "ffffff"
          ">\r\n",
          body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Mini Proxy server</em>\r\n", body);

  // print the HTTP response
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  rio_writen(connfd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  rio_writen(connfd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  rio_writen(connfd, buf, strlen(buf));
  rio_writen(connfd, body, strlen(body));
}
