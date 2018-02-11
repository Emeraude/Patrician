#include "Hud.hpp"
#include "Components.hpp"
#include "systems/Time.hpp"
#include "gui/Text.hpp"

Gui::Hud::Hud(uint16_t width, uint16_t height) : AElement(width, height) {
  _game = new Gui::Game(_width, _height - 20);
  _popin = new Gui::Popin(300, 300);
  _game->setPos(0, 20);
  _children.push_back(_game);
  updateSize(width, height);
}

Gui::Hud::~Hud() {
  for(auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
}

void Gui::Hud::updateSize(uint16_t width, uint16_t height) {
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
  _game->updateSize(_width, _height - 20);
}

void Gui::Hud::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  Ecs::World empty_world;
  Gui::Text *text = new Gui::Text(x, y, content, alignment, "white");
  this->blit(text, empty_world, 0, NULL);
  delete text;
}

SDL_Surface *Gui::Hud::render(Ecs::World& world, uint32_t player, Ecs::Entity *selected) {
  Ecs::Entity *playerEntity = world.get(player);
  uint32_t money = playerEntity->get<comp::Money>()->value;
  std::string const& date = world.get<sys::Time>()->getDate().toString();

  _blitted.clear();
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0, 0, 0));
  SDL_BlitSurface(_sprites["top"], NULL, _surface, NULL);
  this->writeText(std::to_string(money) + "po", 0, 0, Gui::align::LEFT);
  this->writeText(date, _width, 0, Gui::align::RIGHT);
  this->blit(_game, world, player);
  if (selected
      && selected->get<comp::Type>()->type == Type::CITY) {
    this->blit(_popin, world, player, selected);
  }
  return _surface;
}

Ecs::Entity *Gui::Hud::onClickEvent(uint16_t, uint16_t) {
  return NULL;
}
