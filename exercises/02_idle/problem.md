## Idle

OK, it's time to have some **fun with the event loop**.
In this exercise you will learn you to create an event loop and make it run a function when it is idle.

You are provided an `idle_cb` which is the function you want to have run whenever libuv is idle. 
**You do not have to change that function at all.**

Instead you are to fill in the 4 steps outlined in the comments.

You may find the following links useful:

**initializing and starting an idle handler**
- http://docs.libuv.org/en/latest/idle.html

**creating and running the event loop**
- http://docs.libuv.org/en/latest/loop.html

### Hint

Create a `default` loop and Use the `UV_RUN_DEFAULT` mode when running it.

