#include "learnuv.h"
#include <math.h>

const static char* HOST    = "0.0.0.0"; /* localhost */
const static int   PORT    = 7000;
const static int   NBUFS   = 1;         /* number of buffers we write at once */

static uv_tcp_t tcp_server;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

/* forward declarations */
static void close_cb(uv_handle_t* client);
static void server_close_cb(uv_handle_t*);
static void shutdown_cb(uv_shutdown_t*, int);

static void alloc_cb(uv_handle_t*, size_t, uv_buf_t*);
static void read_cb(uv_stream_t*, ssize_t, const uv_buf_t*);
static void write_cb(uv_write_t*, int);

static void close_cb(uv_handle_t* client) {
  free(client);
  log_info("Closed connection");
}

static void shutdown_cb(uv_shutdown_t* req, int status) {
  uv_close((uv_handle_t*) req->handle, close_cb);
  free(req);
}

static void onconnection(uv_stream_t *server, int status) {
  CHECK(status, "onconnection");

  int r = 0;
  uv_shutdown_t *shutdown_req;

  /* 4. Accept client connection */
  log_info("Accepting Connection");

  /* 4.1. Init client connection using `server->loop`, passing the client handle */
  uv_tcp_t *client = malloc(sizeof(uv_tcp_t));
  r = uv_tcp_init(server->loop, client);
  CHECK(r, "uv_tcp_init");

  /* 4.2. Accept the now initialized client connection */
  r = uv_accept(server, (uv_stream_t*) client);
  if (r) {
    log_error("trying to accept connection %d", r);

    shutdown_req = malloc(sizeof(uv_shutdown_t));
    r = uv_shutdown(shutdown_req, (uv_stream_t*) client, shutdown_cb);
    CHECK(r, "uv_shutdown");
  }

  /* 5. Start reading data from client */
  r = uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
  CHECK(r, "uv_read_start");
}

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  /* libuv suggests a buffer size but leaves it up to us to create one of any size we see fit */
  buf->base = malloc(size);
  buf->len = size;
  if (buf->base == NULL) log_error("alloc_cb buffer didn't properly initialize");
}

static void read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
  int r = 0;
  uv_shutdown_t *shutdown_req;

  /* Errors or EOF */
  if (nread < 0) {
    if (nread != UV_EOF) CHECK(nread, "read_cb");

    /* Client signaled that all data has been sent, so we can close the connection and are done */
    free(buf->base);

    shutdown_req = malloc(sizeof(uv_shutdown_t));
    r = uv_shutdown(shutdown_req, client, shutdown_cb);
    CHECK(r, "uv_shutdown");
    return;
  }

  if (nread == 0) {
    /* Everything OK, but nothing read and thus we don't write anything */
    free(buf->base);
    return;
  }

  /* Check if we should quit the server which the client signals by sending "QUIT" */
  if (!strncmp("QUIT", buf->base, fmin(nread, 4))) {
    log_info("Closing the server");
    free(buf->base);
    /* Before exiting we need to properly close the server via uv_close */
    /* We can do this synchronously */
    uv_close((uv_handle_t*) &tcp_server, NULL);
    log_info("Closed server, exiting");
    exit(0);
  }

  /* 6. Write same data back to client since we are an *echo* server and thus can reuse the buffer used to read*/
  /*    We wrap the write req and buf here in order to be able to clean them both later */
  write_req_t *write_req = malloc(sizeof(write_req_t));
  write_req->buf = uv_buf_init(buf->base, nread);
  r = uv_write(&write_req->req, client, &write_req->buf, NBUFS, write_cb);
  CHECK(r, "uv_write");
}

static void write_cb(uv_write_t *req, int status) {
  CHECK(status, "write_cb");

  log_info("Replied to client");

  /* Since the req is the first field inside the wrapper write_req, we can just cast to it */
  /* Basically we are telling C to include a bit more data starting at the same memory location, which in this case is our buf */
  write_req_t *write_req = (write_req_t*) req;
  free(write_req->buf.base);
  free(write_req);
}

int main() {
  int r = 0;
  uv_loop_t *loop = uv_default_loop();

  /* 1. Initialize TCP server */
  r = uv_tcp_init(loop, &tcp_server);
  CHECK(r, "uv_tcp_init");

  /* 2. Bind to localhost:7000 */
  struct sockaddr_in addr;
  r = uv_ip4_addr(HOST, PORT, &addr);
  CHECK(r, "uv_ip4_addr");

  r = uv_tcp_bind(&tcp_server, (struct sockaddr*) &addr, AF_INET);
  CHECK(r, "uv_tcp_bind");

  /* 3. Start listening */
  /* uv_tcp_t inherits uv_stream_t so casting is ok */
  r = uv_listen((uv_stream_t*) &tcp_server, SOMAXCONN, onconnection);
  CHECK(r, "uv_listen");
  log_info("Listening on %s:%d", HOST, PORT);

  uv_run(loop, UV_RUN_DEFAULT);

  MAKE_VALGRIND_HAPPY();

  return 0;
}
