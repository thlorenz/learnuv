## fs readsync

This is the first in a series of exercises that will show us how to read files with libuv.

To start out we will do everything synchronously since it is much simpler to implement. 

Anywhere the API asks for a `uv_fs_cb` we will just pass `NULL` for now. 
Not providing a callback is exactly what makes libuv execute the function synchronously.

However this is **not** the best solution and therefore we will improve on it in the 3 exercises that follow this one.

**Read all the explanations and hints carefully as they apply to the next 3 exercises as well.**

## Main Resources

You should keep the following resources open and refer to them repeatedly:

- [official libuv documentation](http://docs.libuv.org/en/latest/index.html) *you should make use of the "Quick Search" box*
- [Unoffical libuv dox examples](https://github.com/thlorenz/libuv-dox/tree/master/examples)

From now on each exercise will have a **Resource** section towards the bottom of the instruction which guide you to the libuv
API you will need.

## The Exercise

Each step that needs to be completed has been outlined via a comment, i.e.  

```c
/* 1. Open file */
```

You are to implement the missing pieces of each step, usually just **one** call to the libuv API.

As an example:

```c
/* 2. Create buffer and initialize it to turn it into a a uv_buf_t which adds length field */
char buf[BUF_SIZE];
```

is missing the call to `uv_buf_init`, so you'd complete it as follows:

```c
/* 2. Create buffer and initialize it to turn it into a a uv_buf_t which adds length field */
char buf[BUF_SIZE];
uv_buf_t iov = uv_buf_init(buf, sizeof(buf));
```

## Warnings

**Q:** Why do I get all these warnings when I compile? 
**A:** This is due to the fact that not all the variables that are defined are being used yet -- that is your job :)

After you filled in the missing pieces, you shouldn't get any warnings related to **your** code.

## Resources

- [`uv_fs_open`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_open)
- [`uv_buf_init`](http://docs.libuv.org/en/latest/misc.html#c.uv_buf_init)
- [`uv_fs_read`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_read)
- [`uv_fs_close`](http://docs.libuv.org/en/latest/fs.html#c.uv_fs_close)

## Hints

When opening a file pass `O_RDONLY` as the flags and `S_IRUSR` as the mode.

All `uv_fs_*` methods pass back a signed integer that needs to be checked. If it is negative an error occurred.
These checks have been added for you already, but pay close attention as in the future you will need to add them
yourself.
