#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <SDL.h>
#include "AElement.hpp"
#include "Base.hpp"
#include "Game.hpp"
#include "Popin.hpp"

namespace Gui {
  class Hud : public AElement {
  private:
    std::map<std::string, SDL_Surface *> _sprites;
    Gui::Game *_game;
    Gui::Popin *_popin;

  public:
    Hud(uint16_t w, uint16_t h);
    ~Hud();
    void updateSize(uint16_t width, uint16_t height);
    SDL_Surface *render(Ecs::World& world, uint32_t player, Ecs::Entity *selected = NULL);

  private:
    void writeText(std::string const& content, int x, int y, Gui::align alignment = Gui::align::LEFT);
    Ecs::Entity *onClickEvent(uint16_t x, uint16_t y);
  };
}
