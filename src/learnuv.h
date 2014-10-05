#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment the below line when using a terminal that doesn't support ANSI Escape Codes */
/* #define LOG_NOCOLORS 1 */

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "uv.h"

#define CHECK(r, msg) if (r) {                                                 \
  log_error("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r))); \
  exit(1);                                                                     \
}

#endif
