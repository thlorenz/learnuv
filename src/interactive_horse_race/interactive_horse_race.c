#include "interactive_horse_race.h"
#include <math.h>
#include <string.h>

#define TIME_TO_ANSWER 1E7 * 4
#define TRACKS MAX_CLIENTS
#define to_s(x) #x
#define THREADS to_s(TRACKS)

#define HOST  "0.0.0.0" /* localhost */
#define PORT  7000

static void start_game(uv_loop_t* loop, luv_game_t* game) {
  log_info("Initializing track");
  track_init(loop, game->server->clients, game->server->num_clients);
  game->in_progress = 1;
  game->delay = DELAY;
}

static void question_handler(uv_idle_t* handle) {
  luv_game_t *game = handle->data;
  luv_server_t *server = game->server;

  if (!game->in_progress) {
    if (server->num_clients < MAX_CLIENTS) return;
    log_info("Starting the race");
    luv_server_broadcast(server, "\nAll tracks filled, let the race begin!\n");
    start_game(handle->loop, game);
  }
  if (game->question_asked) {
    if (--game->time_to_answer > 0) return;
    luv_server_broadcast(server, "\nWay too slow guys! Next question.\n");
    game->question_asked = 0;
  }

  game->question = luv_questions_get();
  game->question_asked = 1;
  game->time_to_answer = TIME_TO_ANSWER;

  luv_server_broadcast(server, "\n%s\n ? ", game->question.question);
}

static void onclient_connected(luv_client_t* client, int total_connections) {
  luv_server_t *server = client->server;

  /* todo: update track if client gets moved to different slot */
  luv_player_t *player = malloc(sizeof(luv_player_t));
  client->data = player;
  player->client = client;
  player->track = client->slot;

  log_info("New player, %d total now.", total_connections);
  luv_server_broadcast(client->server,
      "Welcome player %d!\nWe now have a total of %d players.\n",
      client->id, total_connections);

  char client_msg[MAX_MSG];
  snprintf(client_msg, MAX_MSG,
    "Welcome to the game, you are on track %d\n", player->track + 1);
  luv_server_send(server, client, client_msg, strlen(client_msg));
}

static void onclient_disconnected(luv_client_t* client, int total_connections) {
  log_info("Player %d quit, %d total now.", client->id, total_connections);
  luv_server_broadcast(client->server,
    "Player quit %d :(\nWe have %d players left.\n",
    client->id, total_connections);
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

  if (!strncasecmp(correct, given, fmin(correct_len, given_len))) {
    player->speed++;

    sprintf(res, "Your answer is correct! Your speed is now %d\n", player->speed);
    game->question_asked = 0;
  } else {
    player->speed = fmax(0, player->speed - 1);
    sprintf(res, "Your answer is wrong! Your speed is now %d\n\n%s\n ? ",player->speed, game->question.question);
  }

  respond(msg, res);
}

int main(void) {
  uv_loop_t *loop = uv_default_loop();

  /* Ensure that each horse gets its own thread, the default libuv threadpool size is 4 */
  setenv("UV_THREADPOOL_SIZE", THREADS, 1);

  srand(time(NULL));
  log_info("Initializing questions");
  luv_questions_init();

  log_info("Creating server");
  luv_server_t server;
  luv_server_init(
      &server
    , loop
    , HOST
    , PORT
    , onclient_connected
    , onclient_disconnected
    , onclient_msg);

  log_info("Starting server");
  luv_server_start(&server, loop);

  log_info("Initializing game loop");
  luv_game_t game = { .server = &server, .delay = DELAY };
  server.data = &game;

  uv_idle_t question_handle;
  uv_idle_init(loop, &question_handle);
  question_handle.data = &game;
  uv_idle_start(&question_handle, question_handler);

  uv_idle_t track_handle;
  uv_idle_init(loop, &track_handle);
  track_handle.data = &game;
  uv_idle_start(&track_handle, track_handler);

  uv_run(loop, UV_RUN_DEFAULT);

  MAKE_VALGRIND_HAPPY();
  return 0;
}
