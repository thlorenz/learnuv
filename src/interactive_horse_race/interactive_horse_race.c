#include "interactive_horse_race.h"
#include <math.h>

#define TIME_TO_ANSWER 1E7 * 3
#define MAX_MSG 256
#define TRACKS MAX_CLIENTS
#define to_s(x) #x
#define THREADS to_s(TRACKS)

#define HOST  "0.0.0.0" // localhost
#define PORT  7000

#define NUM_QUESTIONS 4
const luv_question_t questions[NUM_QUESTIONS] = {
  { "You wake up in a forrest and are surrounded by vines. A gate is to the north." , "N" }     ,
  { "What is the library that powers Node.js"   , "libuv" } ,
  { "1011 1111 in HEX"                          , "BF" }    ,
  { "111 in DECIMAL"                            , "7" }
};

luv_question_t get_question() {
  int i = rand() % NUM_QUESTIONS;
  return questions[i];
}

static void question_handler(uv_idle_t* handle) {
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
  luv_server_t *server = client->server;
  luv_game_t *game = server->data;

  // todo: update track if client gets moved to different slot
  luv_player_t *player = malloc(sizeof(luv_player_t));
  client->data = player;
  player->client = client;
  player->track = client->slot;

  log_info("New player, %d total now.", total_connections);
  char msg[MAX_MSG];
  sprintf(msg, "Welcome player %d!\nWe now have a total of %d players.\n", client->id, total_connections);

  luv_server_broadcast(client->server, msg);
  char client_msg[MAX_MSG];
  sprintf(msg, "Welcome to the game, you are on track %d\n", player->track + 1);
  luv_server_send(server, client, msg);
}

static void onclient_disconnected(luv_client_t* client, int total_connections) {
  log_info("Player %d quit, %d total now.", client->id, total_connections);
  char msg[MAX_MSG];
  sprintf(msg, "Player quit %d :(\nWe have %d players left.\n", client->id, total_connections);
  luv_server_broadcast(client->server, msg);
}

static void onclient_msg(luv_client_msg_t* msg, luv_onclient_msg_processed respond) {
  luv_client_t *client = msg->client;
  log_info("Got message %s from client %d", msg->buf, msg->client->id);

  luv_server_t *server = client->server;
  luv_game_t *game = server->data;
  luv_player_t *player = client->data;

  if (!game->in_progress) {
    respond(msg, "Be patient, the game hasn't started yet.\n");
    return;
  }

  char *correct = game->question.answer;
  char *given = msg->buf;
  int correct_len = strlen(correct);
  int given_len = msg->len;

  char res[MAX_MSG];

  if (!strncmp(correct, given, fmin(correct_len, given_len))) {
    player->speed++;

    sprintf(res, "Your answer is correct! Your speed is now %d\n", player->speed);
    game->question_asked = 0;
  } else {
    player->speed = fmax(0, player->speed - 1);
    sprintf(res, "Your answer is wrong! Your speed is now %d\n", player->speed);
  }

  respond(msg, res);
}

int main(void) {
  uv_loop_t *loop = uv_default_loop();

  /* Ensure that each horse gets its own thread, the default libuv threadpool size is 4 */
  setenv("UV_THREADPOOL_SIZE", THREADS, 1);

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
  server->data = &game;

  uv_idle_t idle_handle;
  uv_idle_init(loop, &idle_handle);
  idle_handle.data = &game;
  uv_idle_start(&idle_handle, question_handler);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
