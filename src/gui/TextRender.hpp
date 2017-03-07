#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "src/Exceptions.hpp"

namespace Gui {
  class TextRender {
  private:
    std::map<std::string, SDL_Color> _colors;
    std::map<std::pair<std::string, uint8_t>, TTF_Font*> _fonts;

  public:
    static TextRender& getInstance();
    SDL_Surface *writeText(std::string const& content,
			   std::string const& font,
			   std::string const& color = "DejaVuSansMono",
			   uint8_t size = 12);

  private:
    TextRender();
    ~TextRender();
    TextRender(TextRender const&);
    void operator=(TextRender const&);
  };
}
