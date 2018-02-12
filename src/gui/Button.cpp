#include "Button.hpp"

Gui::Button::Button(uint16_t w, uint16_t h, std::string const& content) : AElement(w, h), _content(content), _text(NULL) {
  SDL_Surface *in;

  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, _width - 2, _height - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  this->writeText();
  SDL_FreeSurface(in);
}

Gui::Button::~Button() {
  delete _text;
}

Ecs::Entity *Gui::Button::onClickEvent(uint16_t, uint16_t) {
  return NULL;
}

void Gui::Button::writeText() {
  delete _text;
  this->_blitted.clear();
  _text = new Gui::Text(0, 0, _content, Gui::align::MIDDLE_LEFT, "black");
  this->blit(_text);
}

SDL_Surface *Gui::Button::render(Ecs::World&, uint32_t, Ecs::Entity*) {
  return _surface;
}
