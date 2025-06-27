#ifndef FONT_HPP_
#define FONT_HPP_

#include <SDL3_ttf/SDL_ttf.h>

class Font {
  private:
    const char *filename;
    TTF_Font *ttf;
    int size;
    TTF_Font *outline;
    float outline_size;
    // Must be set dynamically with SDL_QueryTexture
    unsigned int line_height;
  public:
    Font();
};

#endif
