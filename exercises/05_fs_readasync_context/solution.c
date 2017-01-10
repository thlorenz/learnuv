#include "learnuv.h"

#define BUF_SIZE 37
static const char *filename = __MAGIC_FILE__;

typedef struct context_struct {
  uv_fs_t *open_req;
  uv_buf_t iov;
} context_t;

void read_cb(uv_fs_t* read_req) {
  int r = 0;
  if (read_req->result < 0) CHECK(read_req->result, "uv_fs_read callback");

  /* extracting our context from the read_req */
  context_t* context = read_req->data;

  /* 4. Report the contents of the buffer */
  log_report("%s", context->iov.base);
  log_info("%s", context->iov.base);

  free(context->iov.base);

  /* 5. Close the file descriptor (synchronously) */
  uv_fs_t close_req;
  r = uv_fs_close(uv_default_loop(), &close_req, context->open_req->result, NULL);
  if (r < 0) CHECK(abs(r), "uv_fs_close");

  /* cleanup all requests and context */
  uv_fs_req_cleanup(context->open_req);
  uv_fs_req_cleanup(read_req);
  uv_fs_req_cleanup(&close_req);
  free(context);
}

void init(uv_loop_t *loop) {
  int r = 0;

  /* No more globals, we need to malloc each request and pass it around for later cleanup */
  uv_fs_t *open_req = malloc(sizeof(uv_fs_t));
  uv_fs_t *read_req = malloc(sizeof(uv_fs_t));

  context_t *context = malloc(sizeof(context_t));
  context->open_req  = open_req;

  /* 1. Open file */
  r = uv_fs_open(loop, open_req, filename, O_RDONLY, S_IRUSR, NULL);
  if (r < 0) CHECK(r, "uv_fs_open");

  /* 2. Create buffer and initialize it to turn it into a a uv_buf_t */
  size_t buf_len = sizeof(char) * BUF_SIZE;
  char *buf = malloc(buf_len);
  context->iov = uv_buf_init(buf, buf_len);

  /* allow us to access the context inside read_cb */
  read_req->data = context;

  /* 3. Read from the file into the buffer */
  r = uv_fs_read(loop, read_req, open_req->result, &context->iov, 1, 0, read_cb);
  if (r < 0) CHECK(r, "uv_fs_read");
}

int main() {
  uv_loop_t *loop = uv_default_loop();

  init(loop);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
