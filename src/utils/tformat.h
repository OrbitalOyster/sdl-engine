#ifndef TFORMAT_H
#define TFORMAT_H

#include "hackery.h"

/*
#define TMSG(s, ...) { \
  va_list ptr; \
  va_start(ptr, VA_NARGS); \
  for (unsigned int = 0; i < VA_NARGS; i++) \
    printf("%s", va_arg(ptr, char*); \
  va_end(ptr); \
  } \
*/


#define TMSG(s, ...) { \
  va_list ptr; \
  va_start(ptr, 3); \
}\

#endif
