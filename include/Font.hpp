#ifndef FONT_HPP_
#define FONT_HPP_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Font {
private:
  SDL_Renderer *renderer;
  const std::string filename;
  TTF_Font *ttf;
  float size;
  TTF_Font *outline;
  float outline_size;

public:
  Font(SDL_Renderer *renderer, const std::string filename, float size,
       float outline_size);
  SDL_Texture *render_text(const std::string text, bool is_outline,
                           SDL_Color color);
  SDL_Texture *render_text(const std::string text, SDL_Color color,
                           SDL_Color outline_color);
  ~Font();
};

#endif
