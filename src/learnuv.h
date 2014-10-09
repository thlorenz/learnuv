#ifndef __LEARNUV_H__
#define __LEARNUV_H__

/* Uncomment the below line when using a terminal that doesn't support ANSI Escape Codes */
/* #define LOG_NOCOLORS 1 */

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "uv.h"

#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_REPORT_LEN 1024

#define CHECK(r, msg) if (r) {                                                 \
  log_error("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r))); \
  exit(1);                                                                     \
}

static char first_report = 1;

/* todo - most likely doesn't work on windows, so investigate http://stackoverflow.com/a/9543049/97443 */
static const char* gethome() {
  struct passwd *pw = getpwuid(getuid());
  return strdup(pw->pw_dir);
}

const char* path_join(const char* p1, const char* p2) {
  char *result = malloc(strlen(p1) + strlen(p2) + 1 + 1);
  strcpy(result, p1);
  strcat(result, "/");
  strcat(result, p2);
  return result;
}

const char* getconfig() {
  const char* home = gethome();
  const char* config = path_join(home, ".config");
  const char* result = path_join(config, "learnuv");
  free((void*)home);
  free((void*)config);
  return result;
}

const char* full_report_path(char* ex_file) {
  /* todo - basename may not be available on Windows, consider http://stackoverflow.com/a/7180746/97443 */
  const char* filename = basename(ex_file);
  const char* config = getconfig();
  const char *result = path_join(config, filename);
  free((void*)config);
  return result;
}

int write_report(const char* path, const char* msg) {
  /* overwrite file every time we run the exercise, but allow multiple reports */
  const char* mode = first_report ? "w+" : "a";
  first_report = 0;
  FILE *f = fopen(path, mode);
  if (f == NULL) {
    log_error("Couldn't open file %s", path);
    return 1;
  }
  fprintf(f,"%s\n", msg);
  fclose(f);
  return 0;
}

#define log_report(M, ...) do {                  \
  char msg[MAX_REPORT_LEN];                      \
  sprintf(msg, M, ##__VA_ARGS__);                \
  const char* path = full_report_path(__FILE__); \
  write_report(path, msg);                       \
  free((void*)path);                             \
} while(0);

#endif
