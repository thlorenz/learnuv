#include "tcp_server.h"

#define TIME_TO_ANSWER 1E7 * 3

#define HOST  "0.0.0.0" // localhost
#define PORT  7000

typedef struct {
  char* question;
  char* answer;
} luv_question_t;

#define NUM_QUESTIONS 4
const luv_question_t questions[NUM_QUESTIONS] = {
  { "You wake up in a forrest and are surrounded by vines. A gate is to the north." , "N" }     ,
  { "What is the library that powers Node.js"   , "libuv" } ,
  { "1011 1111 in HEX"                          , "BF" }    ,
  { "111 in DECIMAL"                            , "7" }
};

typedef struct {
  luv_server_t* server;
  int in_progress;
  int question_asked;
  luv_question_t question;
  int time_to_answer;
} luv_game_t;


luv_question_t get_question() {
  int i = rand() % NUM_QUESTIONS;
  return questions[i];
}

void game_loop(uv_idle_t* handle) {
  luv_game_t *game = handle->data;
  luv_server_t *server = game->server;

  if (!game->in_progress) {
    if (server->num_clients < MAX_CLIENTS) return;
    log_info("Starting the race");
    luv_server_broadcast(server, "\nAll tracks filled, let the race begin!\n");
    game->in_progress = 1;
  }
  if (game->question_asked) {
    if (--game->time_to_answer > 0) return;
    luv_server_broadcast(server, "\nWay too slow guys! Next question.\n");
    game->question_asked = 0;
  }

  game->question = get_question();
  game->question_asked = 1;
  game->time_to_answer = TIME_TO_ANSWER;

  char msg[256];
  sprintf(msg, "\n%s\n ? ", game->question.question);
  luv_server_broadcast(server, msg);
}


static void onclient_connected(luv_client_t* client, int total_connections) {
  log_info("New player, %d total now.", total_connections);
  char msg[256];
  sprintf(msg, "Welcome player %d!\nWe now have a total of %d players.\n", client->id, total_connections);

  luv_server_broadcast(client->server, msg);
}

static void onclient_disconnected(luv_client_t* client, int total_connections) {
  log_info("Player %d quit, %d total now.", client->id, total_connections);
  char msg[256];
  sprintf(msg, "Player quit %d :(\nWe have %d players left.\n", client->id, total_connections);
  luv_server_broadcast(client->server, msg);
}

static void onclient_msg(luv_client_msg_t* msg, luv_onclient_msg_processed respond) {
  log_info("Got message %s from client %d", msg->buf, msg->client->id);
  respond(msg, "RaceTrack: WRONG\n");
}

int main(void) {
  uv_loop_t *loop = uv_default_loop();
  srand(time(NULL));

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

  log_info("Initializing game loop");
  luv_game_t game = { .server = server };

  uv_idle_t idle_handle;
  uv_idle_init(loop, &idle_handle);
  idle_handle.data = &game;
  uv_idle_start(&idle_handle, game_loop);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
