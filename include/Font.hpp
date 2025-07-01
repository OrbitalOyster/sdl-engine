#ifndef FONT_HPP_
#define FONT_HPP_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
private:
  SDL_Renderer *renderer;
  const char *filename;
  TTF_Font *ttf;
  float size;
  TTF_Font *outline;
  float outline_size;
  unsigned int line_height;

public:
  Font(SDL_Renderer *renderer, const char *filename, float size,
       float outline_size);
  SDL_Texture *render_text(const char *text, bool is_outline, SDL_Color color);
  SDL_Texture *render_text(const char *text, SDL_Color color,
                           SDL_Color outline_color);
  ~Font();
};

#endif
