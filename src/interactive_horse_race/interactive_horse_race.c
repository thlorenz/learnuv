#include "tcp_server.h"

#define HOST  "0.0.0.0" // localhost
#define PORT  7000

void onclient_connected(luv_client_t* client, int total_connections) {
  log_info("New player, %d total now.", total_connections);
  luv_server_broadcast(client->server, "Welcome player\n");
}

void onclient_disconnected(luv_client_t* client, int total_connections) {
  log_info("Player %d quit, %d total now.", client->id, total_connections);
  luv_server_broadcast(client->server, "Player quit :(\n");
}

void onclient_msg(luv_client_msg_t* msg, luv_onclient_msg_processed respond) {
  log_info("Got message %s from client %d", msg->buf, msg->client->id);
  respond(msg, "RaceTrack: WRONG");
}

int main(void) {
  uv_loop_t *loop = uv_default_loop();

  log_info("Creating server");
  luv_server_t *server = luv_server_create(
      loop
    , HOST
    , PORT
    , onclient_connected
    , onclient_disconnected
    , onclient_msg);

  log_info("Starting server");
  luv_server_start(server, loop);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
