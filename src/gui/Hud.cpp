#include "Hud.hpp"
#include "Components.hpp"
#include "PlayerBuilder.hpp"
#include "systems/Time.hpp"

Gui::Hud::Hud(uint32_t width, uint32_t height) : _width(width), _height(height), _surface(NULL) {
  //TODO: exceptions
  updateSize(width, height);
  _font = TTF_OpenFont("./resources/fonts/DejaVuSansMono.ttf", 12);
}

Gui::Hud::~Hud() {
  for(auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
  SDL_FreeSurface(_surface);
  TTF_CloseFont(_font);
}

void Gui::Hud::updateSize(uint32_t width, uint32_t height) {
  _width = width;
  _height = height;
  if (_sprites["top"])
    SDL_FreeSurface(_sprites["top"]);
  if (_surface)
    SDL_FreeSurface(_surface);

  _surface = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);
  _sprites["top"] = SDL_CreateRGBSurface(0, _width, 20, 32, 0, 0, 0, 0);

  SDL_Surface *bar = SDL_CreateRGBSurface(0, _width, 1, 32, 0, 0, 0, 0);
  SDL_FillRect(bar, NULL, SDL_MapRGB(bar->format, 127, 127, 127));

  SDL_Rect dst;
  dst.x = 0;
  dst.y = 19;
  SDL_BlitSurface(bar, NULL, _sprites["top"], &dst);
  SDL_FreeSurface(bar);
}

void Gui::Hud::writeText(std::string const& content, int x, int y, std::string alignment) {
  SDL_Rect dst;
  SDL_Color white = {255, 255, 255, 0};
  SDL_Surface *msg= TTF_RenderUTF8_Blended(_font, content.c_str(), white);

  dst.x = x;
  dst.y = y;
  if (alignment == "right")
    dst.x = x - msg->w;
  else if (alignment == "center")
    dst.x = x - msg->w / 2;
  SDL_BlitSurface(msg, NULL, _surface, &dst);
  SDL_FreeSurface(msg);
}

SDL_Surface *Gui::Hud::draw(Ecs::World &world, uint32_t player) {
  Ecs::Entity *playerEntity = world.getEntity(player);
  uint32_t money = playerEntity->getComponent<comp::Money>()->value;
  uint32_t time = world.getSystem<sys::Time>()->getDay();

  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0, 0, 0));
  SDL_BlitSurface(_sprites["top"], NULL, _surface, NULL);
  this->writeText(std::to_string(money) + "po", 0, 0, "left");
  this->writeText(std::string("day ") + std::to_string(time), _width, 0, "right");
  return _surface;
}
