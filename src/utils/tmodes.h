#ifndef TMODES_H
#define TMODES_H

/* List of terminal modes for formatting */

#define _S_ "\x1b["
#define _R_ "\x1b[0m"

#define MODE_NORMAL ";0"
#define MODE_BOLD ";1"
#define MODE_DIM ";2"
#define MODE_ITALIC ";3"
#define MODE_UNDERSCORE ";4"
#define MODE_BLINK ";5"
#define MODE_REVERSE ";7"

#define COLOR_BLACK ";30"
#define COLOR_RED ";31"
#define COLOR_GREEN ";32"
#define COLOR_YELLOW ";33"
#define COLOR_BLUE ";34"
#define COLOR_PURPLE ";35"
#define COLOR_CYAN ";36"
#define COLOR_GRAY ";37"

#define COLOR_DEFAULT ";39"

#define COLOR_BRIGHT_BLACK ";90"
#define COLOR_BRIGHT_RED ";91"
#define COLOR_BRIGHT_GREEN ";92"
#define COLOR_BRIGHT_YELLOW ";93"
#define COLOR_BRIGHT_BLUE ";94"
#define COLOR_BRIGHT_PURPLE ";95"
#define COLOR_BRIGHT_CYAN ";96"
#define COLOR_WHITE ";97"

#endif
