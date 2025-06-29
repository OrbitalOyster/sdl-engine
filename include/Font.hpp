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
    Font(const char *filename, int size, int outline_size);
    SDL_Texture *render_text(SDL_Renderer *renderer, const char *text, SDL_Color color);
    SDL_Texture *render_outline_text(SDL_Renderer *renderer, const char *text, SDL_Color color, SDL_Color outline_color);
    ~Font();
};

#endif
