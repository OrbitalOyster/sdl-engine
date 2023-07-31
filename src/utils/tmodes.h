#ifndef TMODES_H
#define TMODES_H

/* List of terminal modes for formatting */

// Disabled by default, use -DCOLOR_OUTPUT to switch on
#ifdef COLOR_OUTPUT

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

// Simple macro for two-mode formatting
#define TMSG(mode, color, s) _S_ mode color "m" s _R_

#else

#define _S_
#define _R_

#define MODE_NORMAL
#define MODE_BOLD
#define MODE_DIM
#define MODE_ITALIC
#define MODE_UNDERSCORE
#define MODE_BLINK
#define MODE_REVERSE

#define COLOR_BLACK
#define COLOR_RED
#define COLOR_GREEN
#define COLOR_YELLOW
#define COLOR_BLUE
#define COLOR_PURPLE
#define COLOR_CYAN
#define COLOR_GRAY

#define COLOR_DEFAULT

#define COLOR_BRIGHT_BLACK
#define COLOR_BRIGHT_RED
#define COLOR_BRIGHT_GREEN
#define COLOR_BRIGHT_YELLOW
#define COLOR_BRIGHT_BLUE
#define COLOR_BRIGHT_PURPLE
#define COLOR_BRIGHT_CYAN
#define COLOR_WHITE

#define TMSG(mode, color, s) s

#endif

#endif
