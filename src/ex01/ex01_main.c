#include "learnuv.h"

int main() {
  int err;
  
  double uptime;
  err = uv_uptime(&uptime);
  log_info("Uptime %f", uptime);
  CHECK(err, "uv_uptime");
  
  size_t resident_set_memory;
  /* omit below two lines for question - grep code for the CHECK */
  err = uv_resident_set_memory(&resident_set_memory);
  CHECK(err, "uv_resident_set_memory");
  log_info("RSS: %ld", resident_set_memory);
  
  return 0;
}
