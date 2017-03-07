#include "Button.hpp"

Gui::Button::Button(uint32_t w, uint32_t h, std::string const& content) : _w(w), _h(h), _content(content) {
  SDL_Surface *in;

  _surface = SDL_CreateRGBSurface(0, _h, _w, 32, 0, 0, 0, 0);
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, _h - 2, _w - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  this->writeText();
  SDL_FreeSurface(in);
}

Gui::Button::~Button() {
  SDL_FreeSurface(_surface);
}

#include <iostream>
void Gui::Button::click(uint32_t x, uint32_t y) {
  if (x >= _x
      && x < _x + _h
      && y >= _y
      && y < _y + _w)
    std::cout << "Button is clicked!" << std::endl;
}

#define ABS(x) ((int32_t)(x) >= 0 ? x : -(int32_t)(x))
void Gui::Button::writeText() {
  SDL_Surface *msg = Gui::TextRender::getInstance().writeText(_content, "black");
  SDL_Rect dst;

  dst.x = ABS(_h - msg->w) / 2;
  dst.y = ABS(_w - msg->h) / 2;
  SDL_BlitSurface(msg, NULL, _surface, &dst);
  SDL_FreeSurface(msg);
}

void Gui::Button::setPos(uint32_t x, uint32_t y) {
  _x = x;
  _y = y;
}

SDL_Surface *Gui::Button::getSurface() {
  return _surface;
}
