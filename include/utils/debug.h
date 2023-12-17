/*
 * Simple macros for debugging info
 */

#ifndef DEBUG_H
#define DEBUG_H

// Disabled by default, use -DDEBUG_MSG to switch on
#ifdef DEBUG_MSG

#include <stdio.h>
#include <stdlib.h>

#include "tmodes.h"

// Arbitrary number
#define MAX_MSG_SIZE 1024

#define ERR(_debug_c, _debug_s)                                                \
  {                                                                            \
    fprintf(stderr,                                                            \
            TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                         \
                TMSG(MODE_BLINK, COLOR_RED, "%s") "\n",                        \
            __FILE__, __LINE__, _debug_s);                                     \
    exit(_debug_c);                                                            \
  }

#define ERRF(_debug_c, _debug_s, ...)                                          \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    fprintf(stderr,                                                            \
            _S_ MODE_BOLD COLOR_DEFAULT                                        \
            "m[" __FILE__ ":%i] " _R_ _S_ MODE_BLINK COLOR_RED "m%s" _R_ "\n", \
            __LINE__, _debug_b);                                               \
    exit(_debug_c);                                                            \
  }

#define WARN(_debug_s)                                                         \
  fprintf(stderr,                                                              \
          TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                           \
              TMSG(MODE_UNDERSCORE, COLOR_YELLOW, "%s") "\n",                  \
          __FILE__, __LINE__, _debug_s);

#define WARNF(_debug_s, ...)                                                   \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    fprintf(stderr,                                                            \
            _S_ MODE_BOLD COLOR_DEFAULT                                        \
            "m[" __FILE__ ":%i] " _R_ _S_ MODE_UNDERSCORE COLOR_YELLOW         \
            "m%s" _R_ "\n",                                                    \
            __LINE__, _debug_b);                                               \
  }

#define INFO(_debug_s)                                                         \
  fprintf(stdout,                                                              \
          TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                           \
              TMSG(MODE_NORMAL, COLOR_BLUE, "%s") "\n",                        \
          __FILE__, __LINE__, _debug_s);

#define INFOF(_debug_s, ...)                                                   \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    fprintf(stdout,                                                            \
            _S_ MODE_BOLD COLOR_DEFAULT                                        \
            "m[" __FILE__ ":%i] " _R_ _S_ MODE_NORMAL COLOR_BLUE "m%s" _R_     \
            "\n",                                                              \
            __LINE__, _debug_b);                                               \
  }

#define INFO2(_debug_s)                                                        \
  fprintf(stdout,                                                              \
          TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                           \
              TMSG(MODE_NORMAL, COLOR_GREEN, "%s") "\n",                       \
          __FILE__, __LINE__, _debug_s);

#define INFO2F(_debug_s, ...)                                                  \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    fprintf(stdout,                                                            \
            _S_ MODE_BOLD COLOR_DEFAULT                                        \
            "m[" __FILE__ ":%i] " _R_ _S_ MODE_NORMAL COLOR_GREEN "m%s" _R_    \
            "\n",                                                              \
            __LINE__, _debug_b);                                               \
  }

#else /* DEBUG_MSG */
#define ERR(_debug_c, _debug_s) exit(_debug_c);
#define ERRF(_debug_c, _debug_s, ...) exit(_debug_c);
#define WARN(_debug_s)                                                         \
  {}
#define WARNF(_debug_s, ...)                                                   \
  {}
#define INFO(_debug_s)                                                         \
  {}
#define INFOF(_debug_s, ...)                                                   \
  {}
#define INFO2(_debug_s)                                                        \
  {}
#define INFO2F(_debug_s, ...)                                                  \
  {}
#endif /* DEBUG_MSG */

#endif /* DEBUG_H */
