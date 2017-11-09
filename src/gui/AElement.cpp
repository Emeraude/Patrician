#include "AElement.hpp"

Gui::AElement::AElement(uint16_t w, uint16_t h, uint16_t x, uint16_t y) : _width(w), _height(h), _x(x), _y(y) {
  _surface = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);

  if (_surface == NULL)
    throw Exception::Sdl("Unable to create surface");
}

Gui::AElement::~AElement() {
  for (auto* it: _children) {
    delete it;
  }
  SDL_FreeSurface(_surface);
}

Ecs::Entity *Gui::AElement::dispatchClickEvent(uint16_t x, uint16_t y) {
  return this->onClickEvent(x, y);
}

uint16_t Gui::AElement::getWidth() const {
  return _width;
}

uint16_t Gui::AElement::getHeight() const {
  return _height;
}
