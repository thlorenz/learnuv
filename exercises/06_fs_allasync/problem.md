## fs allasync

In this exercise we build up on the context technique we just learned in order to perform **all file operations
asynchronously**.

Our `context` now tracks two requests. 

```c
typedef struct context_struct {
  uv_fs_t *open_req;
  uv_fs_t *read_req;
} context_t;
```

We don't need to track the `close_req` with our `context` since that is available to us when processing our last
invoked callback `close_cb`.

## Forward Declarations

You may be wondering why we are defining our callbacks twice:

```c
void open_cb(uv_fs_t*);
void read_cb(uv_fs_t*);
void close_cb(uv_fs_t*);
```

This is necessary since a function needs to be declared **higher up in the file than it is used**. We could reorder our
callback functions to make that work without forward declarations, but we want to order them to show the flow instead.

## No more Globals at All

If you look closely at the code you'll notice that even our `context` object is no longer global. Instead we `malloc` it
in the `init` method. This also means that we need to deallocate it when we are done.

## Execution Flow and Cleanup

Read through the code carefully and make sure you understand when each piece of code is executed.
The callback functions are ordered by execution to make that easier.

Once you have a feel for how the code works, fill in the missing pieces and be careful to fix all the missing `mallocs`
and `context + request` setups.
Make sure to **use the logger excessively** in order to confirm that things work as you expect.

## Caveat

Be aware that this exercise, like the previous ones, makes the assumption that our `read_cb` is only invoked once. This
works since the buffer we are reading is very small but may not be true when reading larger files.

## Resources

- [`uv_fs_open`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_open)
- [`uv_buf_init`](http://docs.libuv.org/en/latest/misc.html#c.uv_buf_init)
- [`uv_fs_read`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_read)
- [`uv_fs_close`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_close)
- [`uv_read_cb`](http://docs.libuv.org/en/latest/stream.html#c.uv_read_cb)
- [`uv_fs_req_cleanup`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_req_cleanup)
