#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#include "tmodes.h"

#define MAX_ERR_SIZE 1000

#define ERRF(_debug_c, _debug_s, ...)                                          \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                               \
    snprintf(_debug_b, MAX_ERR_SIZE,                                           \
             _S_ MODE_BOLD "m"                                                 \
                           "[%s:%i]" _R_ _S_ MODE_BLINK MODE_BOLD COLOR_RED    \
                           "m"                                                 \
                           " %s" _R_ "\n",                                     \
             __FILE__, __LINE__, _debug_s);                                    \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                    \
    exit(_debug_c);                                                            \
  }

#define WARNF(_debug_s, ...)                                                   \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                               \
    snprintf(_debug_b, MAX_ERR_SIZE,                                           \
             _S_ MODE_BOLD "m"                                                 \
                           "[%s:%i]" _R_ _S_ COLOR_YELLOW "m"                  \
                           " %s" _R_ "\n",                                     \
             __FILE__, __LINE__, _debug_s);                                    \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                    \
  }

#define INFOF(_debug_s, ...)                                                   \
  {                                                                            \
    char _debug_b[MAX_ERR_SIZE];                                               \
    snprintf(_debug_b, MAX_ERR_SIZE,                                           \
             _S_ MODE_BOLD "m"                                                 \
                           "[%s:%i]" _R_ _S_ COLOR_BLUE "m"                    \
                           " %s" _R_ "\n",                                     \
             __FILE__, __LINE__, _debug_s);                                    \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                    \
  }

#endif
