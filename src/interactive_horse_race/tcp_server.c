#include "interactive_horse_race.h"

/* forward declarations */
static void close_cb(uv_handle_t* client);
static void client_shutdown_cb(uv_shutdown_t*, int);
static void shutdown_cb(uv_shutdown_t*, int);

static void alloc_cb(uv_handle_t*, size_t, uv_buf_t*);
static void read_cb(uv_stream_t*, ssize_t, const uv_buf_t*);
static void onclient_msg_processed(luv_client_msg_t*, char*);

static void close_cb(uv_handle_t* client) {
  free(client->data); // free the player variable
  free(client);
  log_info("Closed connection");
}

static void shutdown_cb(uv_shutdown_t* req, int status) {
  uv_close((uv_handle_t*) req->handle, close_cb);
  free(req);
}

static void client_shutdown_cb(uv_shutdown_t* req, int status) {
  uv_close((uv_handle_t*) req->handle, close_cb);
  /* todo: clean client (req->data) and remove it from it's server */
  free(req);
}

static void write_cb(uv_write_t* req, int status) {
  CHECK(status, "write_cb");
  free(req->bufs);
  free(req);
}

static void disconnect(luv_client_t* client) {
  int r;
  luv_server_t* server = client->server;
  int last_slot = server->num_clients - 1;

  /* unless the last client quit we move the last client into its slot
   * in order to keep the clients array contiguous */
  if (client->slot < last_slot) {
    server->clients[client->slot] = server->clients[last_slot];
    server->clients[client->slot]->slot = client->slot;
  }

  server->num_clients--;
  server->onclient_disconnected(client, server->num_clients);

  uv_shutdown_t* shutdown_req = malloc(sizeof(uv_shutdown_t));
  shutdown_req->data = client;
  r = uv_shutdown(shutdown_req, (uv_stream_t*) client, client_shutdown_cb);
  CHECK(r, "uv_shutdown");
}

static void onconnection(uv_stream_t *tcp, int status) {
  CHECK(status, "onconnection");
  luv_server_t* server = (luv_server_t*)tcp;

  int r;

  /* Accept client connection */
  log_info("Accepting Connection");

  if (server->num_clients == MAX_CLIENTS) {
    log_info("exceeded allowed number of clients");
    /* todo: Ideally we'd allow the client to connect,  tell it and disconnect */
    return;
  }

  luv_client_t *client = malloc(sizeof(luv_client_t));
  r = uv_tcp_init(tcp->loop, (uv_tcp_t*)client);
  CHECK(r, "uv_tcp_init");

  r = uv_accept(tcp, (uv_stream_t*) client);
  if (r) {
    log_error("trying to accept connection %d", r);
    disconnect(client);
    return;
  }

  server->clients[server->num_clients] = client;
  client->server = server;
  client->slot   = server->num_clients;
  client->id     = server->ids++;

  server->num_clients++;
  server->onclient_connected(client, server->num_clients);

  /* Start reading data from client */
  r = uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
  CHECK(r, "uv_read_start");
}

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
  if (buf->base == NULL) log_error("alloc_cb buffer didn't properly initialize");
}

static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  luv_client_t *client = (luv_client_t*) stream;
  luv_server_t* server = client->server;

  /* Errors or EOF */
  if (nread < 0) {
    if (nread != UV_EOF) CHECK(nread, "read_cb");
    free(buf->base);
    disconnect(client);
    return;
  }

  if (nread == 0) {
    free(buf->base);
    return;
  }

  luv_client_msg_t *msg = malloc(sizeof(luv_client_msg_t));
  msg->buf    = buf->base;
  msg->len    = nread;
  msg->client = client;

  server->onclient_msg(msg, onclient_msg_processed);
}

static void onclient_msg_processed(luv_client_msg_t* msg, char* response) {
  int r;
  uv_write_t* write_req = malloc(sizeof(uv_write_t));

  int len = strlen(response);
  uv_buf_t buf = uv_buf_init(response, len);
  r = uv_write(write_req, (uv_stream_t*) msg->client, &buf, 1, write_cb);

  free(msg->buf);
}

void luv_server_send(luv_server_t* self, luv_client_t* client, char* msg, int len)
{
  int r;
  uv_write_t* write_req;

  if (client == NULL) {
    log_warn("Client %d was not properly initialized, cannot send message to it.", client->id);
    return;
  }

  uv_buf_t buf = uv_buf_init(msg, len);
  write_req = malloc(sizeof(uv_write_t));
  r = uv_write(write_req, (uv_stream_t*) client, &buf, 1, write_cb);
  CHECK(r, "uv_write");
}

void luv_server_destroy(luv_server_t* self) {
  int r, i;
  uv_shutdown_t *shutdown_req;

  for (i = 0; i < self->num_clients; i++) {
    shutdown_req = malloc(sizeof(uv_shutdown_t));
    shutdown_req->data = self->clients[i];
    r = uv_shutdown(shutdown_req, (uv_stream_t*) self->clients[i], shutdown_cb);
    CHECK(r, "uv_shutdown");
  }

  uv_close((uv_handle_t*) self, NULL);
}

void luv_server_start(luv_server_t* self, uv_loop_t *loop) {
  int r = uv_listen((uv_stream_t*) self, SOMAXCONN, onconnection);
  CHECK(r, "uv_listen");
  log_info("Listening on %s:%d", self->host, self->port);
}

void luv_server_init(
      luv_server_t* self
    , uv_loop_t *loop
    , const char* host
    , int port
    , luv_onclient_connected onclient_connected
    , luv_onclient_disconnected onclient_disconnected
    , luv_onclient_msg onclient_msg) {
  int r;

  self->host                  = host;
  self->port                  = port;
  self->num_clients           = 0;
  self->onclient_connected    = onclient_connected;
  self->onclient_disconnected = onclient_disconnected;
  self->onclient_msg          = onclient_msg;

  /* Initialize TCP server */
  r = uv_tcp_init(loop, &self->tcp);
  CHECK(r, "uv_tcp_init");

  /* Bind to localhost:7000 */
  struct sockaddr_in addr;
  r = uv_ip4_addr(host, port, &addr);
  CHECK(r, "uv_ip4_addr");

  r = uv_tcp_bind(&self->tcp, (struct sockaddr*) &addr, AF_INET);
  CHECK(r, "uv_tcp_bind");
}
