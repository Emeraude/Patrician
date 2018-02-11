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
  for (auto* it: _blitted) {
    if (x >= it->getX()
	&& x < it->getX() + it->getWidth()
	&& y >= it->getY()
	&& y < it->getY() + it->getHeight()) {
      Ecs::Entity *res = it->dispatchClickEvent(x - it->getX(), y - it->getY());
      if (res != NULL)
	return res;
    }
  }
  return this->onClickEvent(x, y);
}

uint16_t Gui::AElement::getWidth() const {
  return _width;
}

uint16_t Gui::AElement::getHeight() const {
  return _height;
}

uint16_t Gui::AElement::getX() const {
  return _x;
}

uint16_t Gui::AElement::getY() const {
  return _y;
}

void Gui::AElement::setX(uint16_t x) {
  _x = x;
}

void Gui::AElement::setY(uint16_t y) {
  _y = y;
}

void Gui::AElement::setPos(uint16_t x, uint16_t y) {
  _x = x;
  _y = y;
}

void Gui::AElement::blit(AElement *e, Ecs::World &world, uint32_t player, Ecs::Entity *selected) {
  SDL_Rect dst;

  dst.x = e->getX();
  dst.y = e->getY();
  SDL_BlitSurface(e->render(world, player, selected), NULL, _surface, &dst);
  _blitted.push_back(e);
}

void Gui::AElement::blit(AElement *e) {
  Ecs::World empty_world;

  this->blit(e, empty_world, 0, NULL);
}
