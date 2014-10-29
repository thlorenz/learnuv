#include "interactive_horse_race.h"

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define HORSE_WIDTH  29
#define HORSE_HEIGHT 11
#define TRACK_WIDTH  110
#define FACTOR       1E7

const static char* horse_pic[HORSE_HEIGHT] = {
  "                 ,***,    ",
  "               ,***` *==--",
  "              ,**`( '|    ",
  "             ,**@ /v_/    ",
  "   ,*.-'''--*** '@@__     ",
  "  **/             |__`v   ",
  " .*'v     |   v   /  //   ",
  " ,*' >   .'----v |  [/    ",
  "    < <<`       ||        ",
  "     `vvv       ||        ",
  "       )vv      )v        "
};

static luv_horse_t horses[] = {
  { .name = "bnoordhuis  "  , .color = 1, .track = 0, .position = 0 },
  { .name = "piscisaureus"  , .color = 2, .track = 1, .position = 0 },
  { .name = "ry          "  , .color = 3, .track = 2, .position = 0 },
  { .name = "saghul      "  , .color = 4, .track = 3, .position = 0 },
  { .name = "indutny     "  , .color = 5, .track = 4, .position = 0 }
};

static void load_color_palette() {
  if (!has_colors()) return;

  use_default_colors();
  start_color();
  init_pair(1, COLOR_BLUE, -1);
  init_pair(2, COLOR_GREEN, -1);
  init_pair(3, COLOR_CYAN, -1);
  init_pair(4, COLOR_RED, -1);
  init_pair(5, COLOR_YELLOW, -1);
}

static void init_screen() {
  initscr();
  curs_set(FALSE);
  raw();
  noecho();
  refresh();
  load_color_palette();
}

static void horse_draw(luv_horse_t *self) {
  if (!DRAW) {
    log_info("Horse %s\ttrack: %d\t position: %d", self->name, self->track, self->position);
    return;
  }

  int i;
  attrset(COLOR_PAIR(self->color));
  for (i = 0; i < HORSE_HEIGHT; i++) {
    mvprintw((self->track * HORSE_HEIGHT) + i, self->position, horse_pic[i]);
  }
  refresh();
}

static void add_player(uv_loop_t* loop, luv_player_t* player) {
  luv_horse_t *horse = horses + player->track;
  player->horse = horse;

  if (!DRAW) log_info("Queued horse %s on track: %d", horse->name, horse->track);
}

static void update_player(luv_player_t* player, int remainder) {
  luv_horse_t *horse = player->horse;

  if (remainder >= player->speed) return;

  /* todo: find better algo for this so both progresses don't happen right after each other */
  log_info("Horse %s progresses", player->horse->name);
  player->horse->position++;
}

void track_handler(uv_idle_t* handle) {
  int i;
  luv_game_t *game = handle->data;
  if (!game->in_progress) return;

  luv_server_t *server = game->server;

  luv_client_t **clients = server->clients;
  int remainder = game->ticks++ % (int)FACTOR;

  for (i = 0; i < server->num_clients; i++)
    update_player(clients[i]->data, remainder);

}

void track_init(uv_loop_t* loop, luv_client_t** clients, int num_players) {
  int i;
  if (DRAW) init_screen();

  for (i = 0; i < num_players; i++)
    add_player(loop, clients[i]->data);
}
