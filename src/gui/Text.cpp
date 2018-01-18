#include "Text.hpp"

Gui::Text::Text(uint16_t x, uint16_t y, std::string const& content, Gui::align alignment, std::string color, std::string fontFamily, uint16_t size) : AElement(0, 0, x, y), _content(content), _alignment(alignment), _color(color), _fontFamily(fontFamily), _size(size), _baseX(x), _baseY(y) {
  this->updateSurface();
  this->setPos(_baseX, _baseY);
}

Gui::Text::~Text() {
  SDL_FreeSurface(_surface);
}

SDL_Surface *Gui::Text::render(Ecs::World &, uint32_t) {
  return _surface;
}

void Gui::Text::updateSurface() {
  delete _surface;
  _surface = TTF_RenderUTF8_Blended(Gui::Text::Cache::getInstance().getFont(_fontFamily, _size),
				    _content.c_str(),
				    Gui::Text::Cache::getInstance().getColor(_color));
  if (_surface == NULL)
    throw Exception::Ttf("Unable to render text");
  _width = _surface->w;
  _height = _surface->h;
}

void Gui::Text::setPos(uint16_t x, uint16_t y) {
  _baseX = _x = x;
  _baseY = _y = y;
  if (_alignment & Gui::align::TOP)
    _y -= _height;
  else if (_alignment & Gui::align::MIDDLE)
    _y -= _height / 2;

  if (_alignment & Gui::align::RIGHT)
    x += _width;
  else if (_alignment & Gui::align::CENTER)
    x += _width /2;
}

Gui::Text::Cache::Cache() {
  if (TTF_Init() == -1)
    throw Exception::Ttf();
  _colors["black"] = {0, 0, 0, 0};
  _colors["white"] = {255, 255, 255, 255};
}

Gui::Text::Cache::~Cache() {
  for (auto& it: _fonts) {
    TTF_CloseFont(it.second);
  }
  TTF_Quit();
}

Gui::Text::Cache& Gui::Text::Cache::getInstance() {
  static Cache cache;
  return cache;
}

SDL_Color& Gui::Text::Cache::getColor(std::string const& name) {
  return _colors.at(name);
}

TTF_Font *Gui::Text::Cache::getFont(std::string const& fontFamily, uint8_t size) {
  if (_fonts[std::pair<std::string, uint8_t>(fontFamily, size)] == NULL) {
    TTF_Font *newFont = TTF_OpenFont((std::string("./resources/fonts/") + fontFamily + ".ttf").c_str(), size);
    if (newFont == NULL)
      throw Exception::Ttf("Unable to load font");
    _fonts[std::pair<std::string, uint8_t>(fontFamily, size)] = newFont;
  }
  return _fonts[std::pair<std::string, uint8_t>(fontFamily, size)];
}
