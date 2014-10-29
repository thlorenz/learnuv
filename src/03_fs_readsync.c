#include "learnuv.h"

#define BUF_SIZE 37
static const char *filename = __MAGIC_FILE__;

int main() {
  int r = 0;
  uv_loop_t *loop = uv_default_loop();

  /* 1. Open file */
  uv_fs_t open_req;

  if (r < 0) CHECK(r, "uv_fs_open");

  /* 2. Create buffer and initialize it to turn it into a a uv_buf_t which adds length field */
  char buf[BUF_SIZE];

  /* 3. Use the file descriptor (the .result of the open_req) to read from the file into the buffer */
  uv_fs_t read_req;

  if (r < 0) CHECK(r, "uv_fs_read");

  /* 4. Report the contents of the buffer */
  log_report("%s", buf);

  log_info("%s", buf);

  /* 5. Close the file descriptor (`open_req.result`) */
  uv_fs_t close_req;

  if (r < 0) CHECK(r, "uv_fs_close");

  /* always clean up your requests when you no longer need them to free unused memory */
  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&read_req);
  uv_fs_req_cleanup(&close_req);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
