## fs readasync

In this exercise you will improve on the previous one.

Although we still will perform most tasks synchronously we will **read** from the file **asynchronously**.

This means that we will pass a callback to the `uv_read` call. This callback is already provided in the code as `read_cb`.
It is also almost complete except for **Closing the file descriptor**.

Additionally a few more steps are missing in the `main` method.
These are almost identical to the previous exercise and thus should be easy to add.

## Resources

- [`uv_fs_open`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_open)
- [`uv_buf_init`](http://docs.libuv.org/en/latest/misc.html#c.uv_buf_init)
- [`uv_fs_read`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_read)
- [`uv_fs_close`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_close)
- [`uv_read_cb`](http://docs.libuv.org/en/latest/stream.html#c.uv_read_cb)
- [`uv_fs_req_cleanup`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_req_cleanup)
