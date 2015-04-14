# horse race 

Are you ready to have some **real** fun now? How about a horse race?
Yes we'll use horses to show you how to use libuv threads.

## The Race

We are simulating a real horse race here. We are also assuming that the horse has to work very hard in order to move.
Therefore for our example we are assuming that moving each horse is CPU intensive, meaning the CPU is constantly busy doing
stuff instead of idling.
Ok, the alert ones of you already spotted the `usleep` statement, but bare with me and lets assume we really have to do
lots of calculations to move a horse ;)

CPU intensive tasks can starve your event loop since no other code can run while the heavy calculations are executing.
Therefore in our horse race we give a thread to each horse. It can be busy on that thread all it wants without affecting
our main thread on which the event loop is running.

Pretty dense? It'll all become clear once you look at the code.

## Creating Threads

Whenever we add a horse we create an async worker, initialize it and put it on the work queue.

**Part of your task** is to complete these steps and I'd suggest looking at the following:

- [`uv_async_init`](http://docs.libuv.org/en/latest/async.html#c.uv_async_init)
- [`uv_queue_work`](http://docs.libuv.org/en/latest/threadpool.html#c.uv_queue_work)

Note that the `race_cb` ("do work"), `finish_cb` and `progress_cb` have already been implemented for you.

Pay close attention when reading the code to see how we keep track of the `async` worker and the context we need.

## Dispatching to the Main Thread

In case you peeked at the code already you may have noticed that we are using `ncurses`. This is a neat little library
that lets us show animations right in the terminal.

There is a catch however. 
Drawing the horses from different threads results in flickering, funky symbols and basically a trashed terminal.
The solution is to draw all horses on the main thread.

But how do we tell the main thread to draw the horse in a different location if it moved?
Have a look at [`uv_async_send`](http://docs.libuv.org/en/latest/async.html#c.uv_async_send).
This method allows us to report progress. It ends up invoking our `progress_cb` on the **main thread**, passing along
our `async` worker. As before we use its the `data` field to pass along the context we need.

Invoking this method in the right place with the right arguments is the **other part of your task**.

## What's the other Code doing

The remaining code just deals with moving the horses in a realistic ;) manner and calling `ncurses` functions to draw
them on the terminal.

If you are interested in `ncurses` have a look at either of the following:

- [ncurses programming howto](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
- [official ncurses page](http://www.gnu.org/software/ncurses/ncurses.html)

