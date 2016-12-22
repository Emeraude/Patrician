#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"

namespace Gui {
  class Game {
  private:
    uint32_t _width;
    uint32_t _height;
    uint32_t _pos_x;
    uint32_t _pos_y;
    SDL_Surface *_surface;
    TTF_Font *_font;
    std::map<std::string, SDL_Surface *> _sprites;

  public:
    Game(uint32_t width, uint32_t height);
    ~Game();
    void updateSize(uint32_t width, uint32_t height);
    void updatePos(int32_t x, int32_t y);
    SDL_Surface *draw(Ecs::World &world, uint32_t player);

  private:
    bool blitSurface(SDL_Surface *surface, uint32_t x, uint32_t y);
    bool blitSurface(SDL_Surface *surface, Ecs::Entity *it);
    bool blitSurface(std::string const& name, uint32_t x, uint32_t y);
    bool blitSurface(std::string const& name, Ecs::Entity *it);
    void writeText(std::string const& content, int x, int y, std::string alignment = "left");
  };
}
