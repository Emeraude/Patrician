#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include "src/Exceptions.hpp"
#include "gui/Base.hpp"
#include "gui/TextRender.hpp"

namespace Gui {
  class Button {
  private:
    uint32_t _x;
    uint32_t _y;
    uint32_t _w;
    uint32_t _h;
    bool _disabled;
    std::string const& _content;
    SDL_Surface *_surface;

  public:
    Button(uint32_t w, uint32_t h, std::string const& content);
    ~Button();
    void click(uint32_t x, uint32_t y);
    void setPos(uint32_t x, uint32_t y);
    SDL_Surface *getSurface();

  private:
    void writeText();
  };
}
