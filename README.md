# Mini Proxy

A Web proxy is a program that acts as a middleman between a Web browser and an end server. Instead of contacting the end server directly to get a Web page, the browser contacts the proxy, which forwards the request on to the end server. When the end server replies to the proxy, the proxy sends the reply on to the browser.

Proxies are useful for many purposes. Sometimes proxies are used in ﬁrewalls, so that browsers behind a ﬁrewall can only contact a server beyond the ﬁrewall via the proxy. Proxies can also act as anonymizers: by stripping requests of all identifying information, a proxy can make the browser anonymous to Web servers. Proxies can even be used to cache web objects by storing local copies of objects from servers then responding to future requests by reading them out of its cache rather than by communicating again with remote servers.

Mini Proxy is a simple HTTP proxy that caches web objects with following
features:

- service many clients in parallel with different ways (I/O multiplexing,
  process-based parallelism, and thread-pool based parallelism).
- speed up performance by implementing a LRU memory cache.

## Architecture 

In this section, we introduce architecture of Mini Proxy application. The
project is organized as follows:

```bash
.
├── app
│   ├── client.c          # client application
│   ├── CMakeLists.txt
│   ├── proxy.c           # proxy application
│   ├── proxy.h           # proxy header file
│   └── reader-writer.c   # reader-writer demo
├── build
├── CMakeLists.txt
├── compile_commands.json # compile database for clang
├── lib
│   ├── CMakeLists.txt
│   ├── include           # include module headers
│   └── src               # include module sources
├── README.md
├── test                  # module tests with googletest
└── googletest            # googletest module

# modules
.
├── CMakeLists.txt
├── include
│   ├── cache.h           # thread-safe cache
│   ├── comm.h            # common definitions
│   ├── csapp.h           # csapp module
│   ├── inet-helper.h     # network module
│   ├── nio.h             # network I/O module
│   ├── proxy-module.h    # thread pipeline
│   └── sbuf.h            # thread-safe message queue
└── src
```

### Thread Pool Model

To service multiple clients in parallel, master-worker flows based on thread-pool model are implemented. There is one master that listens on a port specified by an admin, creates connection when it accepts a connection request, and push connection into the message queue. Sixteen workers appear in the form of thread and retrieve connections from a message queue sitting in the middle between master side and worker side.

Message queue is a canonical producer-consumer model. To prevent multiple workers fetch connection file descriptor (`connfd`) in the message queue, spin lock is employed (refer to `sbuf.c`).

```bash
client ---+                                                +----- worker 0
client ---|     +-----------+    +---------------------+   |      worker 1
          +---> | listen fd | => | connfd, ..., connfd | --+----- worker 2
...       |     +-----------+    +---------------------+   |      ...
          |        master             message queue        +----- worker 15
client ---+       
```

Worker threads are taking care of each connection they retrieved from message
queue (refer to `proxy-module.c:worker_main`).

### Worker Pipeline

Worker pipeline is defined and implemented in `proxy-module` module. Following
work is going to be done sequentially.

- read and parse an HTTP request from client
- read request headers if HTTP request is valid
- respond back to the client with cached response if the request is cached.
- if not, forward an HTTP request/response transaction, cache response and write
  it back to the client.

### LRU Memory Cache

LRU memory cache is implemented with a priority queue based on doubly linked list. Since many threads may read from or write into it in parallel, reader-writer synchronization is a must. We implement a reader-favored approach, i.e. multiple readers are able to read simultaneously, yet only one writer can write when there is no reader reading from the cache (refer to `cache.c`).

## Contributing Changes

Thanks Prof. Randal E. Bryant and Prof. David R. O'Hallaron for opening study materials
to the public.

Contact: quminzhi@gmail.com

This project follows The MIT License (MIT) 
Copyright (c) 2023 Minzhi
