#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include "World.hpp"
#include "Components.hpp"
#include "src/Exceptions.hpp"
#include "gui/AElement.hpp"
#include "gui/Base.hpp"
#include "gui/Button.hpp"
#include "gui/Text.hpp"

namespace Gui {
  class Popin : public Gui::AElement {
  private:
    Ecs::Entity *_city;
    std::map<Resource, Gui::Button *> _buttons;

  public:
    Popin(uint16_t w, uint16_t h);
    ~Popin();

    SDL_Surface *render(Ecs::World &world, uint32_t cityId, Ecs::Entity *selected = NULL);
    Ecs::Entity *onClickEvent(uint16_t x, uint16_t y);

  private:
    void writeText(std::string const& content, int x, int y, Gui::align alignment = Gui::align::LEFT);
  };
}
