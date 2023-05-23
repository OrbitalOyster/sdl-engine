#ifndef COLORS_H
#define COLORS_H

#define COLOR_RED    "[0;31m"
#define COLOR_GREEN  "[0;32m"
#define COLOR_YELLOW "[0;33m"
#define COLOR_BLUE   "[0;34m"
#define COLOR_PURPLE "[0;35m"
#define COLOR_CYAN   "[0;36m"

#ifdef COLOR_OUTPUT
#define MSG_RED(s) "\033" COLOR_RED s "\033[0m"
#define MSG_GREEN(s) "\033" COLOR_GREEN s "\033[0m"
#define MSG_YELLOW(s) "\033" COLOR_YELLOW s "\033[0m"
#define MSG_BLUE(s) "\033" COLOR_BLUE s "\033[0m"
#define MSG_PURPLE(s) "\033" COLOR_PURPLE s "\033[0m"
#define MSG_CYAN(s) "\033" COLOR_CYAN s "\033[0m"
#else
#define MSG_RED(s) s
#define MSG_GREEN(s) s
#define MSG_YELLOW(s) s
#define MSG_BLUE(s) s
#define MSG_PURPLE(s) s
#define MSG_CYAN(s) s
#endif

#endif
