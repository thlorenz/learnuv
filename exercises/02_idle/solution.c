#include "learnuv.h"

void idle_cb(uv_idle_t* handle) {
  static int64_t count = -1;
  count++;
  if ((count % 10000) == 0) log_report(".");
  if (count >= 50000) uv_idle_stop(handle);
}

int main() {
  uv_idle_t idle_handle;

  /* 1. create the event loop */
  uv_loop_t *loop = uv_default_loop();

  /* 2. init an idle handler for the loop */
  uv_idle_init(loop, &idle_handle);

  /* 3. start the idle handler with a function to call */
  uv_idle_start(&idle_handle, idle_cb);

  /* 4. start the event loop */
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
