#pragma once

#include <cstdint>
#include <list>
#include <SDL.h>
#include "World.hpp"
#include "../Exceptions.hpp"

namespace Gui {
  class AElement {
  protected:
    std::list<AElement *> _children;
    SDL_Surface *_surface;
    uint16_t _width;
    uint16_t _height;
    uint16_t _x;
    uint16_t _y;

  public:
    AElement(uint16_t w, uint16_t h, uint16_t x = 0, uint16_t y = 0);
    virtual ~AElement();
    virtual SDL_Surface *render(Ecs::World& w, uint32_t player) = 0;

    Ecs::Entity *dispatchClickEvent(uint16_t x, uint16_t y);
    uint16_t getWidth() const;
    uint16_t getHeight() const;

  protected:
    virtual Ecs::Entity *onClickEvent(uint16_t x, uint16_t y) = 0;
  };
}
