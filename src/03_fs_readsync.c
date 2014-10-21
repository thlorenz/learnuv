#include "learnuv.h"
/* do not change the first line of this file as it will affect the result */

#define BUF_SIZE 37
static const char *filename = __MAGIC_FILE__;

  //const char *filename = path_join(getconfig(), "magic_file.txt");
int main() {
  int r;
  uv_loop_t *loop = uv_default_loop();

  /* 1. Open file */
  uv_fs_t open_req;
  r = uv_fs_open(loop, &open_req, filename, O_RDONLY, S_IRUSR, NULL);
  if (r < 0) CHECK(abs(r), "uv_fs_open");

  /* 2. Create buffer and initialize it to turn it into a a uv_buf_t which adds lenght property */
  char buf[BUF_SIZE];
  uv_buf_t iov = uv_buf_init(buf, sizeof(buf));

  /* 3. Use the file descriptor (the .result of the open_req) to read from the file into the buffer */
  uv_fs_t read_req;
  r = uv_fs_read(loop, &read_req, open_req.result, &iov, 1, 0, NULL);
  if (r < 0) CHECK(abs(r), "uv_fs_read");

  /* 4. Report the contents of the buffer */
  log_report("%s", buf);

  log_info("%s", buf);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
