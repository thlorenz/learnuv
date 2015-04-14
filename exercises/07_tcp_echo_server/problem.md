# TCP Echo Server

This one is loads of fun albeit a little more involved as well.
We are going to build a TCP server in C -- yes you heard me right!

## What We Will Learn

**!!TONS!!**, after all lots of techniques and library functions go into building such a thing in pure C.
Possibly you'll come out of this exercise realizing why JavaScript is such a good fit for asynchronous programming ;),
but no pain no gain right?

So lets start with the easy part.

### Starting the Server and Listening on a Port

First of all we need to initialize our server `tcp_server`. 
This is done via [`uv_tcp_init`](http://docs.libuv.org/en/latest/tcp.html#c.uv_tcp_init).
Our server represents the TCP `handle`.

Next we need to obtain a socket address for the given host and port and then bind it to our server.
You should have a look at the following to methods provided by libuv:

- [`uv_ip4_addr`](http://docs.libuv.org/en/latest/misc.html#c.uv_ip4_addr)
- [`uv_tcp_bind`](http://docs.libuv.org/en/latest/tcp.html#c.uv_tcp_bind)

Finally we listen for connections via
[`uv_listen`](http://docs.libuv.org/en/latest/stream.html#c.uv_listen).

### C Inheritance Technique

Note that `uv_listen` actually takes a `uv_stream_t*` argument since it works not only `uv_tcp_t*` types, but any
stream.
This works because `uv_tcp_t` inherits `uv_stream_t` via a technique explained in the introduction to this workshopper.

You should make sure you review how this works by investigating the following:

- [`uv_tcp_t`](http://docs.libuv.org/en/latest/tcp.html)
- [`uv_stream_t`](http://docs.libuv.org/en/latest/stream.html)
- [`uv_handle_t`](http://docs.libuv.org/en/latest/handle.html)

We are using this technique in our code as well to wrap the `uv_write_req` in order to add the `buf` field:

```c
typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;
```

Since `uv_write_t req` is the first field, when we store a `write_req_t` at a memory location we can cast back and forth
between both types.

### Your Task

You are to fill in the missing code snippets in order to complete the TCP server. However at least as important is for
you to carefully read through the code and understand the concepts and techniques used.

## Hint

Use `nc localhost 7000` to test your server (finish via `Ctrl-D`) and/or stop the server by sending `QUIT`. 
You can also send entire files, i.e. `cat package.json | netcat localhost 7000`.

On windows you may have to use telnet or putty.

## Resources (not yet mentioned)

- [`uv_tcp_init`](http://docs.libuv.org/en/latest/tcp.html#c.uv_tcp_init)
- [`uv_accept`](http://docs.libuv.org/en/latest/stream.html#c.uv_accept)
- [`uv_read_start`](http://docs.libuv.org/en/latest/stream.html#c.uv_read_start)
- [`uv_close`](http://docs.libuv.org/en/latest/handle.html#c.uv_close)

## Verification

When your server is working please start it up and run `./learnuv verify` in another terminal.
