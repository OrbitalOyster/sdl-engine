#ifndef OOPS_H
#define OOPS_H

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

#define OOPS(s, c)                                                             \
  {                                                                            \
    fprintf(stderr, "[%s:%i] %s", __FILE__, __LINE__, MSG_RED(s));             \
    exit(c);                                                                   \
  }

#define WARN(s)                                                                \
  fprintf(stderr, "[%s:%i] %s", __FILE__, __LINE__, MSG_YELLOW(s));

#endif

