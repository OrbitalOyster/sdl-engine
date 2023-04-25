#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_MSG

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

#define MAX_ERR_SIZE 1000

#define ERR(_debug_c, _debug_s)                                                              \
  {                                                                            \
    fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_RED(_debug_s));           \
    exit(_debug_c);                                                                   \
  }

#define ERRF(_debug_c, _debug_s, ...)                                                        \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                                      \
    snprintf(_debug_b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_RED(_debug_s)); \
    fprintf(stderr, _debug_b, __VA_ARGS__);                                           \
    exit(_debug_c);                                                                   \
  }

#define WARN(_debug_s)                                                                \
  fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_YELLOW(_debug_s));

#define WARNF(_debug_s, ...)                                                          \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                                      \
    snprintf(_debug_b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__,              \
             MSG_YELLOW(_debug_s));                                                   \
    fprintf(stderr, _debug_b, __VA_ARGS__);                                           \
  }

#define INFO(_debug_s) printf("[%s:%i] %s\n", __FILE__, __LINE__, MSG_BLUE(_debug_s));

#define INFOF(_debug_s, ...)                                                          \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                                      \
    snprintf(_debug_b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__,              \
             MSG_BLUE(_debug_s));                                                     \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                           \
  }

#else
#define ERR(_debug_c, _debug_s)
#define ERRF(_debug_c, _debug_s, ...)
#define WARN(_debug_s)
#define WARNF(_debug_s, ...)
#define INFO(_debug_s)
#define INFOF(_debug_s, ...)
#endif

#endif
