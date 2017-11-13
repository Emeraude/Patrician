#pragma once

#include <cstdint>
#include <map>
#include <list>
#include <string>
#include <SDL.h>
#include "World.hpp"
#include "src/Exceptions.hpp"
#include "gui/AElement.hpp"
#include "gui/Base.hpp"
#include "gui/TextRender.hpp"

namespace Gui {
  class Game : public Gui::AElement {
  private:
    uint16_t _pos_x;
    uint16_t _pos_y;
    std::list<std::pair<SDL_Rect, Ecs::Entity *>> _blittedEntities;
    std::map<std::string, SDL_Surface *> _sprites;

  public:
    Game(uint16_t width, uint16_t height);
    ~Game();
    void updateSize(uint16_t width, uint16_t height);
    void updatePos(int32_t x, int32_t y);
    SDL_Surface *render(Ecs::World &world, uint32_t player);

  private:
    bool blitSurface(SDL_Surface *surface, uint32_t x, uint32_t y);
    bool blitSurface(SDL_Surface *surface, Ecs::Entity *it);
    bool blitSurface(std::string const& name, uint32_t x, uint32_t y);
    bool blitSurface(std::string const& name, Ecs::Entity *it);
    void writeText(std::string const& content, int x, int y, Gui::align alignment = Gui::align::LEFT);

    Ecs::Entity *onClickEvent(uint16_t x, uint16_t y);
  };
}
