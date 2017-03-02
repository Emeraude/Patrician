#include "Button.hpp"

Gui::Button::Button(uint32_t w, uint32_t h, std::string const& content) : _w(w), _h(h), _content(content) {
  SDL_Surface *in;

  _font = TTF_OpenFont("./resources/fonts/DejaVuSansMono.ttf", 12);
  if (_font == NULL)
    throw Exception::Ttf("Unable to load font");

  _surface = SDL_CreateRGBSurface(0, h, w, 32, 0, 0, 0, 0);
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 127, 127, 127));
  in = SDL_CreateRGBSurface(0, h - 2, w - 2, 32, 0, 0, 0, 0);
  SDL_FillRect(in, NULL, SDL_MapRGB(in->format, 255, 255, 255));

  SDL_Rect dst;
  dst.x = 1;
  dst.y = 1;
  SDL_BlitSurface(in, NULL, _surface, &dst);

  this->writeText(_content, _w / 2, _h / 2, Gui::align::MIDDLE_CENTER);
  SDL_FreeSurface(in);
}

Gui::Button::~Button() {
  SDL_FreeSurface(_surface);
  TTF_CloseFont(_font);
}

#include <iostream>
void Gui::Button::click(uint32_t x, uint32_t y) {
  if (x >= _x
      && x < _x + _h
      && y >= _y
      && y < _y + _w)
    std::cout << "Button is clicked!" << std::endl;
}

void Gui::Button::writeText(std::string const& content, int x, int y, Gui::align alignment) {
  SDL_Color black = {0, 0, 0, 0};
  SDL_Surface *msg= TTF_RenderUTF8_Blended(_font, content.c_str(), black);
  SDL_Rect dst;

  if (alignment & Gui::align::TOP)
    y -= msg->h;
  else if (alignment & Gui::align::MIDDLE)
    y -= msg->h / 2;

  if (alignment & Gui::align::RIGHT)
    x -= msg->w;
  else if (alignment & Gui::align::CENTER)
    x -= msg->w / 2;
  dst.y = x;
  dst.x = y;
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
