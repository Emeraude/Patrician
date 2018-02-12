#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include "src/Exceptions.hpp"
#include "gui/AElement.hpp"
#include "gui/Base.hpp"
#include "gui/Text.hpp"

namespace Gui {
  class Button : public Gui::AElement {
  private:
    bool _disabled;
    std::string const& _content;
    Gui::Text *_text;

  public:
    Button(uint16_t w, uint16_t h, std::string const& content);
    ~Button();

    SDL_Surface *render(Ecs::World& w, uint32_t player, Ecs::Entity *selected = NULL);
    Ecs::Entity *onClickEvent(uint16_t x, uint16_t y);

  private:
    void writeText();
  };
}
