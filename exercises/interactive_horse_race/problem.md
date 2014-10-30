# interactive horse race 

This is the one exercise where all you have to do is **have fun**! You earned it!

## Launching the horse race

Start the `interactive_horse_race` and then connect **5** players via `nc localhost 7000`.
Once enough players are connected, the race begins.

## The rules

Each player gets asked the same question and whoever answers first gets to whip his horse and make it a bit faster.
If however the answer is wrong the horse becomes slower.

Whoever reaches the end of the track first wins.

## What can you learn here

This is the exercise where a lot of things we learned before come together to create this silly little game.
One thing to notice is that now **we don't use threads for our horses** since really we aren't calculating much to move
them. 

Instead we use `idle` callbacks (remember the second exercise) in order to update their position. This of course means
we are doing so on the main thread and can draw the horse without dispatching.

We register another `idle` callback in order to ask the questions when we need to.

## Project structure

We are using separate modules for the different features we need, namely the TCP server, the questions provider, the
horse track and the race itself

All types and common functions are defined inside `interactive_horse_race.h`. Here you'll also find `DRAW` which you can
set to `0` in order to log information instead of drawing horses - just in case you are interested how things work ;)

The TCP server connects the players, asks them questions and receives answers.
Notice how we use the `EventEmitter` pattern you should know from Node.js in order to notify the game about client
interaction:

```c
struct luv_server_s {
  uv_tcp_t      tcp;
  [..]
  /* events */
  luv_onclient_connected onclient_connected;
  luv_onclient_disconnected onclient_disconnected;
  luv_onclient_msg onclient_msg;
};
```

I highly encourage you to dig through the code to see how the techniques, libuv features and patterns we learned are
used throughout the code.

## Not Production Ready

The very keen of you may have detected some memory leaks in our game. You are perfectly right. We are not properly
`free` ing everything we allocate.

I challenge you to fix them using [valgrind](http://valgrind.org/) to detect them.

I'll be honest with you though. Properly cleaning up what you allocate can be quite challenging especially in async code
we use with libuv. 
We have seen how to do this properly for smaller examples, however in this case I just did not have the time to do this
properly for our horse race.

So even though I'd be happy for any PRs that fixes this our motto here is:
**A leaky horse race is better than no horse race at all** and I'm sure any Gambler will agree :)

Another "feature" that some may not like is due to how we handle disconnects. Should that happen in the middle of the
game, a player's horse may be changed from right under him.
