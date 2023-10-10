/**
 * "d" stands for "dullard"
 */

#include <stdio.h>
#include <time.h>

#include "utils/tmodes.h"

#define _dinclude(f) #f
#define dinclude(f) _dinclude(f)

#define DTEST_UNIT_START(s)                                                    \
  printf(TMSG(MODE_BOLD, COLOR_DEFAULT, "\tTesting unit: ")                    \
             TMSG(MODE_ITALIC, COLOR_GRAY, "%s\n"),                            \
         s);                                                                   \
  fflush(stdout);                                                              \
  unsigned int _testsRun = 0;                                                  \
  unsigned int _ok = 0;                                                        \
  unsigned int _failed = 0;

#define DTEST_UNIT_END                                                         \
  printf(TMSG(MODE_BOLD, COLOR_DEFAULT,                                        \
              "\tDone. Tests: %u; Passed: %u; Failed: %u\n"),                  \
         _testsRun, _ok, _failed);

#define DTEST_EVAL_TIME(s)                                                     \
  {                                                                            \
    printf("\t\tEval time: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT, #s) " |");   \
    clock_t _startTime = clock();                                              \
    s;                                                                         \
    double _result = (double)(clock() - _startTime) / CLOCKS_PER_SEC;          \
    printf("%.4lf sec.\n", _result);                                           \
  }

#define DTEST_EVAL_DOUBLE(s)                                                   \
  {                                                                            \
    double _result = s;                                                        \
    printf(                                                                    \
        "\t\tEval: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT, #s) " | %25.25lf\n", \
        _result);                                                              \
  }

#define DTEST_EXPECT_TRUE(s)                                                   \
  {                                                                            \
    printf("\t\tTest: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT,                   \
                             #s) " | Expected: true | ");                      \
    int _result = s;                                                           \
    printf("Result: %i | ", _result);                                          \
    if (_result) {                                                             \
      _ok++;                                                                   \
      printf(TMSG(MODE_BOLD, COLOR_GREEN, "OK\n"));                            \
    } else {                                                                   \
      _failed++;                                                               \
      printf(TMSG(MODE_BLINK, COLOR_RED, "FAIL\n"));                           \
    }                                                                          \
    _testsRun++;                                                               \
  }

#define DTEST_EXPECT_FALSE(s)                                                  \
  {                                                                            \
    printf("\t\tTest: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT,                   \
                             #s) " | Expected: false | ");                     \
    int _result = s;                                                           \
    printf("Result: %i | ", _result);                                          \
    if (!_result) {                                                            \
      _ok++;                                                                   \
      printf(TMSG(MODE_BOLD, COLOR_GREEN, "OK\n"));                            \
    } else {                                                                   \
      _failed++;                                                               \
      printf(TMSG(MODE_BLINK, COLOR_RED, "FAIL\n"));                           \
    }                                                                          \
    _testsRun++;                                                               \
  }

#include dinclude(TEST_FILE)
