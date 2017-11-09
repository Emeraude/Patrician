#include "Game.hpp"
#include "Components.hpp"

Gui::Game::Game(uint16_t width, uint16_t height) : AElement(width, height), _pos_x(0), _pos_y(0) {
  updateSize(width, height);

  _sprites["city"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["city"], NULL, SDL_MapRGB(_sprites["city"]->format, 255, 0, 0));
  _sprites["ship"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["ship"], NULL, SDL_MapRGB(_sprites["ship"]->format, 255, 255, 255));
}

Gui::Game::~Game() {
  for (auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
}

void Gui::Game::updateSize(uint32_t width, uint32_t height) {
  _width = width;
  _height = height;
  if (_surface)
    SDL_FreeSurface(_surface);

  _surface = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);
}

void Gui::Game::updatePos(int32_t x, int32_t y) {
  if ((int32_t)_pos_x + x >= (int32_t)(1000 - _width)) {
    if ((int32_t)(1000 - _width) < 0)
      _pos_x = 0;
    else
      _pos_x = 1000 - _width;
  }
  else if ((int32_t)_pos_x + x <= 0)
    _pos_x = 0;
  else
    _pos_x += x;

  if ((int32_t)_pos_y + y >= (int32_t)(1000 - _height)) {
    if ((int32_t)(1000 - _height) < 0)
      _pos_y = 0;
    else
      _pos_y = 1000 - _height;
  }
  else if ((int32_t)_pos_y + y <= 0)
    _pos_y = 0;
  else
    _pos_y += y;
}

bool Gui::Game::blitSurface(SDL_Surface *surface, uint32_t x, uint32_t y) {
  SDL_Rect dst;

  if (x > _pos_x
      && y > _pos_y
      && x < _pos_x + _width
      && y < _pos_y + _height) {
    dst.x = x - _pos_x;
    dst.y = y - _pos_y;
    SDL_BlitSurface(surface, NULL, _surface, &dst);
    return true;
  }
  return false;
}

bool Gui::Game::blitSurface(SDL_Surface *surface, Ecs::Entity *e) {
  comp::Position *pos = e->get<comp::Position>();
  bool blitted = blitSurface(surface, pos->x, pos->y);
  SDL_Rect dst;

  dst.x = pos->x - _pos_x;
  dst.y = pos->y - _pos_y;
  dst.w = surface->w;
  dst.h = surface->h;
  _blittedEntities.push_back(std::pair<SDL_Rect, Ecs::Entity*>(dst, e));
  return blitted;
}

bool Gui::Game::blitSurface(std::string const& name, uint32_t x, uint32_t y) {
  return blitSurface(_sprites[name], x, y);
}

bool Gui::Game::blitSurface(std::string const& name, Ecs::Entity *e) {
  return blitSurface(_sprites[name], e);
}

void Gui::Game::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  SDL_Surface *msg = Gui::TextRender::getInstance().writeText(content, "white");

  if (alignment & Gui::align::TOP)
    y -= msg->h;
  else if (alignment & Gui::align::MIDDLE)
    y -= msg->h / 2;

  if (alignment & Gui::align::RIGHT)
    x -= msg->w;
  else if (alignment & Gui::align::CENTER)
    x -= msg->w / 2;
  blitSurface(msg, x, y);
  SDL_FreeSurface(msg);
}

SDL_Surface *Gui::Game::render(Ecs::World &world, uint32_t player) {
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0, 0, 0));
  _blittedEntities.clear();
  for (auto *it: world.getEntities()) {
    if (it->get<comp::Type>()->type == Type::CITY) {
      blitSurface("city", it);
      comp::Position *pos = it->get<comp::Position>();
      writeText(it->get<comp::Name>()->value.c_str(),
		pos->x, pos->y, Gui::align::TOP_CENTER);
    }
    else if (it->get<comp::Type>()->type == Type::SHIP) {
      blitSurface("ship", it);
    }
  }
  return _surface;
}

Ecs::Entity *Gui::Game::onClickEvent(uint16_t x, uint16_t y) {
  for (auto& it: _blittedEntities) {
    if (x >= it.first.x
	&& x < it.first.x + it.first.w
	&& y >= it.first.y
	&& y < it.first.y + it.first.h) {
      return it.second;
    }
  }
  return NULL;
}
