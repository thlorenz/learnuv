#ifndef __LEARNUV_H__
#define __LEARNUV_H__

/* Uncomment the below line when using a terminal that doesn't support ANSI Escape Codes */
/* #define LOG_NOCOLORS 1 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "log.h"
#include "uv.h"
#include "task.h" /* MAKE_VALGRIND_HAPPY */

#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_REPORT_LEN 1024

#define CHECK(r, msg) if (r) {                                                       \
  log_error("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), (int) r, uv_strerror((r))); \
  exit(1);                                                                           \
}

#ifndef __MAGIC_FILE__
#define __MAGIC_FILE__ "Please build using using ./learnuv make or similar."
#endif

#ifndef __LEARNUV_CONFIG__
#define __LEARNUV_CONFIG__ "Please build using using ./learnuv make or similar."
#endif

static char first_report = 1;

static const char* path_join(const char* p1, const char* p2) {
  char *result = malloc(strlen(p1) + strlen(p2) + 1 + 1);
  strcpy(result, p1);
  strcat(result, "/");
  strcat(result, p2);
  return result;
}

static const char* full_report_path(char* ex_file) {
  /* todo - basename may not be available on Windows, consider http://stackoverflow.com/a/7180746/97443 */
  const char* filename = basename(ex_file);
  return path_join(__LEARNUV_CONFIG__, filename);
}

static int write_report(const char* path, const char* msg) {
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
