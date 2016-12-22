#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"

namespace Gui {
  class Hud {
  private:
    uint32_t _width;
    uint32_t _height;
    SDL_Surface *_surface;
    TTF_Font *_font;
    std::map<std::string, SDL_Surface *> _sprites;

  public:
    Hud(uint32_t width, uint32_t height);
    ~Hud();
    void updateSize(uint32_t width, uint32_t height);
    SDL_Surface *draw(Ecs::World &world, uint32_t player);

  private:
    void writeText(std::string const& content, int x, int y, std::string alignment);
  };
}
