# what is libuv

- multi-platform support library with a focus on asynchronous I/O 
- developed for Node.js but used elsewhere
  - Mozilla's Rust language
  - Luvit 
  - Julia
  - pyuv

# libuv node.js powerhouse

![img](https://raw.githubusercontent.com/thlorenz/learnuv/gh-pages/img/nodejs-architecture.png)

# history

![img](https://raw.githubusercontent.com/thlorenz/learnuv/gh-pages/img/shit.png)

# history

- Node.js originally used **libev** 
- libev relies on kqueue or (e)poll only supported by UNIXes
- **libuv** started as abstraction
  - libev for UNIX
  - IOCP for Windows

# programming model

- interface to the underlying system
  - perform asynchronous file system operations
  - access TCP or UDP sockets
  - listen to file system events
- provides an event-loop implementation backed by kernel eventing mechanisms

# programming model

- async APIs
- heavy use of callbacks
- structured inheritance chain for request types

# c 

- some features in C heavily used
- lets look at some of them in detail

# pointers 

![pointer](https://raw.githubusercontent.com/thlorenz/learnuv/gh-pages/img/pointer.png)

# interpreting memory

![8-16-bit](https://raw.githubusercontent.com/thlorenz/learnuv/gh-pages/img/8-16-bit.png)

- what's in memory is *in the eye of the beholder*
- C types tell it how to interpret memory and how much of it to read
- can allocate 16 bits (2 bytes) and have it read as 8 bits which yields different result

# pointer arithmetic

- `sizeof(char) == 1 byte`
- `sizeof(short) == 2 bytes`
- `char *c = 0x00` `c + 1 == 0x01`
- `short *s = 0x00` `s + 1 == 0x02`

# memory based inheritance

```c
/* uv.h */

/*
 * uv_tcp_t is a subclass of uv_stream_t.
 *
 * Represents a TCP stream or TCP server.
 */
struct uv_tcp_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  int ipc;
};
```

*For simplicity we are ignoring the fact that `uv_ipc_t` defines more fields than just `int ipc`*

# memory based inheritance

![inheritance](https://raw.githubusercontent.com/thlorenz/learnuv/gh-pages/img/inheritance.png)

# Keeping Track

```c
void read_cb(uv_fs_t* read_req);
```
- *callback style* code without *closures* is hard
- all you may access here is the `read_req`

# Keeping Track

- fortunately there is a `void* data` property on pretty much everything so you can attach your context
- any libuv *request* type will have the following fields

```c
#define UV_REQ_FIELDS           \
  /* public */                  \
  void* data;                   \
  [..]                          \
  UV_REQ_PRIVATE_FIELDS         \
```


# Callbacks to implement  Event Emitters

- when working with **libuv** you may encounter callbacks that are meant to be invoked more than once
- this allows us to build simple event emitters *supporting one subscriber per event*

```c
struct luv_server_s {
  uv_tcp_t      tcp;
  [..]
  void*         data;
  /* events */
  luv_onclient_connected onclient_connected;
  luv_onclient_disconnected onclient_disconnected;
  luv_onclient_msg onclient_msg;
};
```

# Callbacks to implement Event Emitters

```c
luv_server_t* luv_server_create(
      uv_loop_t *loop
    , const char* host
    , int port
    , luv_onclient_connected onclient_connected
    , luv_onclient_disconnected onclient_disconnected
    , luv_onclient_msg onclient_msg) {

  luv_server_t *self          = malloc(sizeof(luv_server_t));
  self->host                  = host;
  self->port                  = port;
  self->num_clients           = 0;
  self->onclient_connected    = onclient_connected;
  self->onclient_disconnected = onclient_disconnected;
  self->onclient_msg          = onclient_msg;
}
```

# The End

Now go and have fun with the workshop.

You might wanna build and run the `interactive_horse_race` just to get yourself motivated ;)
