#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"
#include "Components.hpp"
#include "src/Exceptions.hpp"
#include "gui/Base.hpp"

namespace Gui {
  class Popin {
  private:
    uint32_t _w;
    uint32_t _h;
    SDL_Surface *_surface;
    TTF_Font *_font;

  public:
    Popin(uint32_t w, uint32_t h);
    ~Popin();

    SDL_Surface *draw(Ecs::World &world, Ecs::Entity *city);

  private:
    void writeText(std::string const& content, int x, int y, Gui::align alignment = Gui::align::LEFT);
  };
}
