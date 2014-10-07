#include "learnuv.h"

int main() {
  int err;

  double uptime;
  err = uv_uptime(&uptime);
  CHECK(err, "uv_uptime");
  log_info("Uptime: %f", uptime);
  log_report("Uptime: %f", uptime);

  size_t resident_set_memory;

  return 0;
}
