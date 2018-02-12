#include "Popin.hpp"

Gui::Popin::Popin(uint16_t w, uint16_t h) : Gui::AElement(w, h), _city(NULL) {
  for (uint32_t i = Resource::FIRST; i <= Resource::LAST; ++i) {
    Gui::Button *btn = new Gui::Button(15, 15, "<");
    btn->setPos(130, 50 + i * 17);
    _buttons[(Resource)i] = btn;
  }
}

Gui::Popin::~Popin() {
  for (auto &it: _buttons) {
    delete it.second;
  }
}

void Gui::Popin::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  Gui::Text text = Gui::Text(x, y, content, alignment, "black");
  this->blit(&text);
}

SDL_Surface *Gui::Popin::render(Ecs::World &world, uint32_t cityId, Ecs::Entity *selected) {
  SDL_Surface *in;

  this->_blitted.clear();
  _city = selected ? selected : world.get(cityId);
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
  comp::Stock *stock = e->get<comp::Stock>();
  for (uint32_t i = Resource::FIRST; i <= Resource::LAST; ++i) {
    this->writeText(std::string(infosResource[i].name)
		    + " "
		    + std::to_string(stock->at(static_cast<Resource>(i)).quantity),
		    10, 50 + 17 * i);
    this->blit(_buttons[(Resource)i]);
  }
  return _surface;
}

Ecs::Entity *Gui::Popin::onClickEvent(uint16_t, uint16_t) {
  return _city;
}
