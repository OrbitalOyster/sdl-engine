/**
 * "d" stands for "dullard"
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "utils/tmodes.h"

// Hackery for including macros within macros
#define _dinclude(f) #f
#define dinclude(f) _dinclude(f)

#define DTEST_UNIT_START(s)                                                    \
  printf(TMSG(MODE_BOLD, COLOR_DEFAULT, "\tTesting unit: ")                    \
             TMSG(MODE_ITALIC, COLOR_GRAY, "%s\n"),                            \
         s);                                                                   \
  fflush(stdout);                                                              \
  unsigned int _tests_run = 0;                                                 \
  unsigned int _ok = 0;                                                        \
  unsigned int _failed = 0;

#define DTEST_UNIT_END                                                         \
  printf(TMSG(MODE_BOLD, COLOR_DEFAULT,                                        \
              "\tDone. Tests: %u; Passed: %u; Failed: %u\n"),                  \
         _tests_run, _ok, _failed);                                            \
  return (int)_failed;

#define DTEST_INFO(s) printf("\t\t* " TMSG(MODE_BOLD, COLOR_BLUE, s) " *\n");

#define DTEST_INFOF(s, ...)                                                    \
  printf("\t\t* " TMSG(MODE_BOLD, COLOR_BLUE, s) " *\n", __VA_ARGS__);

#define DTEST_EVAL_TIME(s)                                                     \
  {                                                                            \
    printf("\t\tEval time: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT, #s) " | ");  \
    fflush(stdout);                                                            \
    clock_t _start_time = clock();                                             \
    s;                                                                         \
    double _result = (double)(clock() - _start_time) / CLOCKS_PER_SEC;         \
    printf("%.6lf sec.\n", _result);                                           \
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
                             #s) " | Expected: true  | ");                     \
    int _result = s;                                                           \
    printf("Result: %i | ", _result);                                          \
    if (_result) {                                                             \
      _ok++;                                                                   \
      printf(TMSG(MODE_BOLD, COLOR_GREEN, "OK\n"));                            \
    } else {                                                                   \
      _failed++;                                                               \
      printf(TMSG(MODE_BLINK, COLOR_RED, "FAIL\n"));                           \
    }                                                                          \
    fflush(stdout);                                                            \
    _tests_run++;                                                              \
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
    fflush(stdout);                                                            \
    _tests_run++;                                                              \
  }

#define DTEST_EXPECT_UINT(s, uint)                                             \
  {                                                                            \
    printf("\t\tTest: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT,                   \
                             #s) " | Expected: %u | ",                         \
           uint);                                                              \
    unsigned int _result = s;                                                  \
    printf("Result: %u | ", _result);                                          \
    if (_result == uint) {                                                     \
      _ok++;                                                                   \
      printf(TMSG(MODE_BOLD, COLOR_GREEN, "OK\n"));                            \
    } else {                                                                   \
      _failed++;                                                               \
      printf(TMSG(MODE_BLINK, COLOR_RED, "FAIL\n"));                           \
    }                                                                          \
    fflush(stdout);                                                            \
    _tests_run++;                                                              \
  }

#define DTEST_EXPECT_STRING(_test, _str)                                       \
  {                                                                            \
    printf("\t\tTest: " TMSG(MODE_UNDERSCORE, COLOR_DEFAULT,                   \
                             #_test) " | Expected: %s | ",                     \
           #_str);                                                             \
    char *_result = _test;                                                     \
    printf("Result: \"%s\" | ", _result);                                      \
    if (!strcmp(_result, _str)) {                                              \
      _ok++;                                                                   \
      printf(TMSG(MODE_BOLD, COLOR_GREEN, "OK\n"));                            \
    } else {                                                                   \
      _failed++;                                                               \
      printf(TMSG(MODE_BLINK, COLOR_RED, "FAIL\n"));                           \
    }                                                                          \
    fflush(stdout);                                                            \
    _tests_run++;                                                              \
  }

#include dinclude(TEST_FILE)
