#include "Game.hpp"
#include "Components.hpp"

//TODO: exceptions
Gui::Game::Game(uint32_t width, uint32_t height) : _width(width), _height(height), _pos_x(0), _pos_y(0), _surface(NULL) {
  updateSize(width, height);
  _font = TTF_OpenFont("./resources/fonts/DejaVuSansMono.ttf", 12);
  _sprites["city"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["city"], NULL, SDL_MapRGB(_sprites["city"]->format, 255, 0, 0));
  _sprites["ship"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["ship"], NULL, SDL_MapRGB(_sprites["ship"]->format, 255, 255, 255));
}

Gui::Game::~Game() {
  for (auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
  SDL_FreeSurface(_surface);
  TTF_CloseFont(_font);
}

void Gui::Game::updateSize(uint32_t width, uint32_t height) {
  _width = width;
  _height = height;
  if (_surface)
    SDL_FreeSurface(_surface);

  _surface = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);
}

void Gui::Game::updatePos(int32_t x, int32_t y) {
  if ((int32_t)_pos_x + x >= (int32_t)(1000 - _width))
    _pos_x = 1000 - _width;
  else if ((int32_t)_pos_x + x <= 0)
    _pos_x = 0;
  else
    _pos_x += x;

  if ((int32_t)_pos_y + y >= (int32_t)(1000 - _height))
    _pos_y = 1000 - _height;
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
  comp::Position *pos = e->getComponent<comp::Position>();
  return blitSurface(surface, pos->x, pos->y);
}

bool Gui::Game::blitSurface(std::string const& name, uint32_t x, uint32_t y) {
  return blitSurface(_sprites[name], x, y);
}

bool Gui::Game::blitSurface(std::string const& name, Ecs::Entity *e) {
  comp::Position *pos = e->getComponent<comp::Position>();
  return blitSurface(name, pos->x, pos->y);
}


void Gui::Game::writeText(std::string const& content, int x, int y, std::string alignment) {
  SDL_Color white = {255, 255, 255, 0};
  SDL_Surface *msg= TTF_RenderUTF8_Blended(_font, content.c_str(), white);

  //TODO: replace by a macro (bitwise)
  if (alignment == "top"
      || alignment == "top-right"
      || alignment == "top-center")
    y -= msg->h;
  if (alignment == "right"
      || alignment == "top-right")
    x -= - msg->w;
  else if (alignment == "center"
	   || alignment == "top-center")
    x -= msg->w / 2;
  blitSurface(msg, x, y);
  SDL_FreeSurface(msg);
}

SDL_Surface *Gui::Game::draw(Ecs::World &world, uint32_t player) {
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0, 0, 0));
  for (auto *it: world.getEntities()) {
    if (it->getComponent<comp::Type>()->type == Type::CITY) {
      blitSurface("city", it);
      comp::Position *pos = it->getComponent<comp::Position>();
      writeText(it->getComponent<comp::Name>()->value.c_str(),
		pos->x, pos->y, "top-center");
    }
    else if (it->getComponent<comp::Type>()->type == Type::SHIP) {
      blitSurface("ship", it);
    }
  }
  return _surface;
}
