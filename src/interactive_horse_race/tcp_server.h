#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "learnuv.h"

#define MAX_CLIENTS 2

typedef struct luv_server_s luv_server_t;

typedef struct {
  uv_tcp_t  connection;
  int       id;
  int       slot;
  luv_server_t* server;
} luv_client_t;

typedef struct {
  char*           buf;
  size_t          len;
  luv_client_t*   client;
} luv_client_msg_t;

typedef void (*luv_onclient_msg_processed)(luv_client_msg_t*, char*);
typedef void (*luv_onclient_msg)(luv_client_msg_t*, luv_onclient_msg_processed);
typedef void (*luv_onclient_connected)(luv_client_t*, int);
typedef void (*luv_onclient_disconnected)(luv_client_t*, int);

/* server inherits uv_tcp_t since its the first field */
struct luv_server_s {
  uv_tcp_t      tcp;
  const char*   host;
  int           port;
  luv_client_t* clients[MAX_CLIENTS];
  int           num_clients;
  int           ids;
  /* events */
  luv_onclient_connected onclient_connected;
  luv_onclient_disconnected onclient_disconnected;
  luv_onclient_msg onclient_msg;
};

void luv_server_broadcast(luv_server_t*, char*);
void luv_server_destroy(luv_server_t*);
void luv_server_start(luv_server_t*, uv_loop_t*);

luv_server_t* luv_server_create(
    uv_loop_t *loop
  , const char* host
  , int port
  , luv_onclient_connected onclient_connected
  , luv_onclient_disconnected onclient_disconnected
  , luv_onclient_msg onclient_msg);

#endif
