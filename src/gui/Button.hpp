#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include "src/Exceptions.hpp"
#include "gui/AElement.hpp"
#include "gui/Base.hpp"
#include "gui/TextRender.hpp"

namespace Gui {
  class Button : public Gui::AElement {
  private:
    bool _disabled;
    std::string const& _content;

  public:
    Button(uint16_t w, uint16_t h, std::string const& content);
    Ecs::Entity *onClickEvent(uint16_t x, uint16_t y);
    void setPos(uint16_t x, uint16_t y);
    SDL_Surface *render(Ecs::World& w, uint32_t player);

  private:
    void writeText();
  };
}
