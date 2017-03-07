#include "Popin.hpp"

Gui::Popin::Popin(uint32_t w, uint32_t h) : _w(w), _h(h), _surface(NULL) {}

Gui::Popin::~Popin() {
  if (_surface)
    SDL_FreeSurface(_surface);
}

void Gui::Popin::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  SDL_Surface *msg = Gui::TextRender::getInstance().writeText(content, "black");

  if (alignment & Gui::align::TOP)
    y -= msg->h;
  else if (alignment & Gui::align::MIDDLE)
    y -= msg->h / 2;

  if (alignment & Gui::align::RIGHT)
    x -= msg->w;
  else if (alignment & Gui::align::CENTER)
    x -= msg->w / 2;

  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  SDL_BlitSurface(msg, NULL, _surface, &dst);
  SDL_FreeSurface(msg);
}

SDL_Surface *Gui::Popin::draw(Ecs::World &world, Ecs::Entity *city) {
  SDL_Surface *in;
  if (_surface)
    SDL_FreeSurface(_surface);

  _surface = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, _w - 2, _h - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  SDL_FreeSurface(in);

  std::string cityName = city->getComponent<comp::Name>()->value;
  this->writeText(cityName, _w / 2, 10, Gui::align::CENTER);

  Ecs::Entity *e = world.getEntity(city->getComponent<comp::Buildings>()->office);
  comp::Stock *s = e->getComponent<comp::Stock>();
  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    this->writeText(std::string(infosResource[i].name) + " " + std::to_string(s->at(static_cast<Resource>(i)).quantity),
		    10, 50 + 15 * i);
  }

  return _surface;
}

uint32_t Gui::Popin::getW() const {
  return _w;
}

uint32_t Gui::Popin::getH() const {
  return _h;
}

#include <iostream>
void Gui::Popin::click(uint32_t x, uint32_t y) {
  std::cout << "Popin is clicked" << std::endl;
}
