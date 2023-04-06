#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_MSG

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

#define MAX_ERR_SIZE 1000

#define ERR(c, s)                                                              \
  {                                                                            \
    fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_RED(s));           \
    exit(c);                                                                   \
  }

#define ERRF(c, s, ...)                                                        \
  {                                                                            \
    char b[MAX_ERR_SIZE];                                                      \
    snprintf(b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_RED(s)); \
    fprintf(stderr, b, __VA_ARGS__);                                           \
    exit(c);                                                                   \
  }

#define WARN(s)                                                                \
  fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, MSG_YELLOW(s));

#define WARNF(s, ...)                                                          \
  {                                                                            \
    char b[MAX_ERR_SIZE];                                                      \
    snprintf(b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__,              \
             MSG_YELLOW(s));                                                   \
    fprintf(stderr, b, __VA_ARGS__);                                           \
  }

#define INFO(s) printf("[%s:%i] %s\n", __FILE__, __LINE__, MSG_BLUE(s));

#define INFOF(s, ...)                                                          \
  {                                                                            \
    char b[MAX_ERR_SIZE];                                                      \
    snprintf(b, MAX_ERR_SIZE, "[%s:%i] %s\n", __FILE__, __LINE__,              \
             MSG_BLUE(s));                                                     \
    fprintf(stdout, b, __VA_ARGS__);                                           \
  }

#else
#define ERR(c, s)
#define ERRF(c, s, ...)
#define WARN(s)
#define WARNF(s, ...)
#define INFO(s)
#define INFOF(s, ...)
#endif

#endif
