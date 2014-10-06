#include "learnuv.h"

void idle_cb(uv_idle_t* handle) {
  static int64_t count = -1;
  count++;
  if ((count % 10000) == 0) log_report(".");
  if (count >= 50000) uv_idle_stop(handle);
}

int main() {
  uv_idle_t idle_handle;

  /* remove all below lines for exercise */
  uv_loop_t *loop = uv_default_loop();
  uv_idle_init(loop, &idle_handle);
  uv_idle_start(&idle_handle, idle_cb);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
