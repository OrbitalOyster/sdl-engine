#ifndef HACKERY_H
#define HACKERY_H

#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#endif
