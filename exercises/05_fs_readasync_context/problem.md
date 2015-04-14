## fs readasync context

In this exercise we introduce a technique that allows you to keep track of your requests via a `context`.
In this case our context only has one field, but lets start small shall we?

None of our requests are global anymore, only our `context` is.

### malloc

Have a closer look at the `init` function.

```c
void init(uv_loop_t *loop) {
  uv_fs_t *open_req = malloc(sizeof(uv_fs_t));
  uv_fs_t *read_req = malloc(sizeof(uv_fs_t));

  [..]
}
```

We need to `malloc` these requests because if we would use **automatic variables** instead, they would be
"automatically" deallocated once we leave the `init` function body.
However we need them to stay around since the `read_cb` will be invoked asynchronously.

### Keeping Track of Requests

The tricky part is maintaining a reference to our requests since we need to deallocate that memory once our read operation is
completed. That's what the context is for. We use it to add all the requests we need to track.

In order to access the context once the `read_cb` callback is invoked, we attach it to the `data` field of the
`read_req` like so: `read_req->data = context`. That is exactly what this field is for and why it is a `void*` pointer,
**allowing us to point it at anything**.

Then inside the `read_cb` we get our context back from that `data` field: `context_t* context = read_req->data`. This is
a common technique used in libraries using libuv and even in libuv itself.

**Make sure to fully understand this technique** before moving to the next exercise as we will build up on it.

## Resources

- [`uv_fs_open`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_open)
- [`uv_buf_init`](http://docs.libuv.org/en/latest/misc.html#c.uv_buf_init)
- [`uv_fs_read`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_read)
- [`uv_fs_close`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_close)
- [`uv_read_cb`](http://docs.libuv.org/en/latest/stream.html#c.uv_read_cb)
- [`uv_fs_req_cleanup`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_req_cleanup)
