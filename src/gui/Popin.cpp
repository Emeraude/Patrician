#include "Popin.hpp"

Gui::Popin::Popin(uint16_t w, uint16_t h) : Gui::AElement(w, h), _city(NULL) {
  _button = new Gui::Button(80, 20, "Wild button");
}

Gui::Popin::~Popin() {
  delete _button;
}

void Gui::Popin::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  Gui::Text text = Gui::Text(x, y, content, alignment, "black");
  this->blit(&text);
}

SDL_Surface *Gui::Popin::render(Ecs::World &world, uint32_t cityId, Ecs::Entity *selected) {
  SDL_Surface *in;

  this->_blitted.clear();
  if (selected)
    _city = selected;
  else
    _city = world.get(cityId);
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, _width - 2, _height - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  SDL_FreeSurface(in);

  std::string cityName = _city->get<comp::Name>()->value;
  this->writeText(cityName, _width / 2, 10, Gui::align::CENTER);

  Ecs::Entity *e = world.get(_city->get<comp::Buildings>()->office);
  comp::Stock *s = e->get<comp::Stock>();
  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    this->writeText(std::string(infosResource[i].name) + " " + std::to_string(s->at(static_cast<Resource>(i)).quantity),
		    10, 50 + 15 * i);
  }
  this->blit(_button, world, 0, NULL);

  return _surface;
}

#include <iostream>
Ecs::Entity *Gui::Popin::onClickEvent(uint16_t x, uint16_t y) {
  std::cout << "Popin is clicked" << std::endl;
  return _city;
}
