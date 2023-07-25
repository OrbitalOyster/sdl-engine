#ifndef COLORS_H
#define COLORS_H

#define COLOR_BLACK         "[0;30m"
#define COLOR_RED           "[0;31m"
#define COLOR_GREEN         "[0;32m"
#define COLOR_YELLOW        "[0;33m"
#define COLOR_BLUE          "[0;34m"
#define COLOR_PURPLE        "[0;35m"
#define COLOR_CYAN          "[0;36m"
#define COLOR_GRAY          "[0;37m"

#define COLOR_DEFAULT       "[0;39m"

#define COLOR_BRIGHT_BLACK  "[0;90m"
#define COLOR_BRIGHT_RED    "[0;91m"
#define COLOR_BRIGHT_GREEN  "[0;92m"
#define COLOR_BRIGHT_YELLOW "[0;93m"
#define COLOR_BRIGHT_BLUE   "[0;94m"
#define COLOR_BRIGHT_PURPLE "[0;95m"
#define COLOR_BRIGHT_CYAN   "[0;96m"
#define COLOR_WHITE         "[0;97m"

#ifdef COLOR_OUTPUT
#define MSG_BLACK(s)  "\033" COLOR_BLACK  s "\033[0m"
#define MSG_RED(s)    "\033" COLOR_RED    s "\033[0m"
#define MSG_GREEN(s)  "\033" COLOR_GREEN  s "\033[0m"
#define MSG_YELLOW(s) "\033" COLOR_YELLOW s "\033[0m"
#define MSG_BLUE(s)   "\033" COLOR_BLUE   s "\033[0m"
#define MSG_PURPLE(s) "\033" COLOR_PURPLE s "\033[0m"
#define MSG_CYAN(s)   "\033" COLOR_CYAN   s "\033[0m"
#define MSG_GRAY(s)   "\033" COLOR_GRAY   s "\033[0m"

#define MSG_BRIGHT_BLACK(s)  "\033" COLOR_BRIGHT_BLACK  s "\033[0m"
#define MSG_BRIGHT_RED(s)    "\033" COLOR_BRIGHT_RED    s "\033[0m"
#define MSG_BRIGHT_GREEN(s)  "\033" COLOR_BRIGHT_GREEN  s "\033[0m"
#define MSG_BRIGHT_YELLOW(s) "\033" COLOR_BRIGHT_YELLOW s "\033[0m"
#define MSG_BRIGHT_BLUE(s)   "\033" COLOR_BRIGHT_BLUE   s "\033[0m"
#define MSG_BRIGHT_PURPLE(s) "\033" COLOR_BRIGHT_PURPLE s "\033[0m"
#define MSG_BRIGHT_CYAN(s)   "\033" COLOR_BRIGHT_CYAN   s "\033[0m"
#define MSG_WHITE(s)         "\033" COLOR_WHITE         s "\033[0m"

#else
#define MSG_BLACK(s) s
#define MSG_RED(s) s
#define MSG_GREEN(s) s
#define MSG_YELLOW(s) s
#define MSG_BLUE(s) s
#define MSG_PURPLE(s) s
#define MSG_CYAN(s) s
#define MSG_GRAY(s) s
#define MSG_BRIGHT_BLACK(s) s
#define MSG_BRIGHT_RED(s) s
#define MSG_BRIGHT_GREEN(s) s
#define MSG_BRIGHT_YELLOW(s) s
#define MSG_BRIGHT_BLUE(s) s
#define MSG_BRIGHT_PURPLE(s) s
#define MSG_BRIGHT_CYAN(s) s
#define MSG_WHITE(s) s
#endif

#endif
