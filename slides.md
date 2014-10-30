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

# C 


# Pointers 

# Memory bases Inheritance

- inheritance (pointers + memory addresses, pointer arithmetic) -- show libuv examples

# Keeping Track

- callbacks + contexts
- `void*` and the data field

# Callbacks vs. EventEmitter
