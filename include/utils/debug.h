#ifndef DEBUG_H
#define DEBUG_H

/* Simple macros for debugging info */

// Disabled by default, use -DDEBUG_MSG to switch on
#ifdef DEBUG_MSG

#include <stdio.h>
#include <stdlib.h>

#include "tmodes.h"

// Arbitrary number
#define MAX_MSG_SIZE 1024

/**
 * Macro doesn't check for format discrepancies, so INFOF("%lf", 1) will
 * output 0.000000; SNPRINTF_CHECK will force an extra sprintf to raise an
 * error on wrong formatting
 */

#define SNPRINTF_CHECK

#ifdef SNPRINTF_CHECK
#define FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, ...)                    \
  snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__)
#else
#define FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, ...)
#endif

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
    FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    char *_tmsg = TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                   \
        TMSG(MODE_BLINK, COLOR_RED, "%s") "\n";                                \
    snprintf(_debug_b, MAX_MSG_SIZE, _tmsg, __FILE__, __LINE__, _debug_s);     \
    fprintf(stderr, _debug_b, __VA_ARGS__);                                    \
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
    FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    char *_tmsg = TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                   \
        TMSG(MODE_UNDERSCORE, COLOR_YELLOW, "%s") "\n";                        \
    snprintf(_debug_b, MAX_MSG_SIZE, _tmsg, __FILE__, __LINE__, _debug_s);     \
    fprintf(stderr, _debug_b, __VA_ARGS__);                                    \
  }

#define INFO(_debug_s)                                                         \
  fprintf(stdout,                                                              \
          TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                           \
              TMSG(MODE_NORMAL, COLOR_BLUE, "%s") "\n",                        \
          __FILE__, __LINE__, _debug_s);

#define INFOF(_debug_s, ...)                                                   \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);               \
    char *_tmsg = TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                   \
        TMSG(MODE_NORMAL, COLOR_BLUE, "%s") "\n";                              \
    snprintf(_debug_b, MAX_MSG_SIZE, _tmsg, __FILE__, __LINE__, _debug_s);     \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                    \
  }

#define INFO2(_debug_s)                                                        \
  fprintf(stdout,                                                              \
          TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                           \
              TMSG(MODE_NORMAL, COLOR_GREEN, "%s") "\n",                       \
          __FILE__, __LINE__, _debug_s);

#define INFO2F(_debug_s, ...)                                                  \
  {                                                                            \
    char _debug_b[MAX_MSG_SIZE];                                               \
    FORMAT_CHECK(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);               \
    snprintf(_debug_b, MAX_MSG_SIZE, _debug_s, __VA_ARGS__);                   \
    char *_tmsg = TMSG(MODE_BOLD, COLOR_DEFAULT, "[%s:%i] ")                   \
        TMSG(MODE_NORMAL, COLOR_GREEN, "%s") "\n";                             \
    snprintf(_debug_b, MAX_MSG_SIZE, _tmsg, __FILE__, __LINE__, _debug_s);     \
    fprintf(stdout, _debug_b, __VA_ARGS__);                                    \
  }

#else
#define ERR(_debug_c, _debug_s)
#define ERRF(_debug_c, _debug_s, ...)
#define WARN(_debug_s)
#define WARNF(_debug_s, ...)
#define INFO(_debug_s)
#define INFOF(_debug_s, ...)
#define INFO2(_debug_s)
#define INFO2F(_debug_s, ...)
#endif

#endif
