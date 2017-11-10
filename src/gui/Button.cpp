#include "Button.hpp"

Gui::Button::Button(uint16_t w, uint16_t h, std::string const& content) : AElement(w, h), _content(content) {
  SDL_Surface *in;

  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, _height - 2, _width - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  this->writeText();
  SDL_FreeSurface(in);
}

#include <iostream>
Ecs::Entity *Gui::Button::onClickEvent(uint16_t x, uint16_t y) {
  std::cout << "Button is clicked at " << x << ":" << y << "!" << std::endl;
  return NULL;
}

#define ABS(x) ((int16_t)(x) >= 0 ? x : -(int16_t)(x))
void Gui::Button::writeText() {
  SDL_Surface *msg = Gui::TextRender::getInstance().writeText(_content, "black");
  SDL_Rect dst;

  dst.x = ABS(_height - msg->w) / 2;
  dst.y = ABS(_width - msg->h) / 2;
  SDL_BlitSurface(msg, NULL, _surface, &dst);
  SDL_FreeSurface(msg);
}

SDL_Surface *Gui::Button::render(Ecs::World&, uint32_t) {
  return _surface;
}
