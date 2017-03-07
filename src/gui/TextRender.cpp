#include "TextRender.hpp"

Gui::TextRender::TextRender() {
  if (TTF_Init() == -1)
    throw Exception::Ttf();
  _colors["black"] = {0, 0, 0, 0};
  _colors["blue"] = {0, 0, 255, 0};
  _colors["green"] = {0, 255, 0, 0};
  _colors["red"] = {255, 0, 0, 0};
  _colors["white"] = {255, 255, 255, 0};
}

Gui::TextRender::~TextRender() {
  for (auto& it: _fonts) {
    TTF_CloseFont(it.second);
  }
  TTF_Quit();
}

Gui::TextRender& Gui::TextRender::getInstance() {
  static TextRender instance;
  return instance;
}

SDL_Surface *Gui::TextRender::writeText(std::string const& content,
		       std::string const& color,
		       std::string const& font,
		       uint8_t size) {
  if (_fonts[std::pair<std::string, uint8_t>(font, size)] == NULL) {
    TTF_Font *newFont = TTF_OpenFont((std::string("./resources/fonts/") + font + ".ttf").c_str(), size);
    if (newFont == NULL)
      throw Exception::Ttf("Unable to load font");
    _fonts[std::pair<std::string, uint8_t>(font, size)] = newFont;
  }
  SDL_Surface *msg = TTF_RenderUTF8_Blended(_fonts[std::pair<std::string, uint8_t>(font, size)],
					    content.c_str(),
					    _colors[color]);
  return msg;
}
