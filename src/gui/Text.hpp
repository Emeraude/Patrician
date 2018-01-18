#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"
#include "gui/AElement.hpp"
#include "gui/Base.hpp"
#include "gui/TextRender.hpp"
#include "src/Exceptions.hpp"

namespace Gui {
  class Text : public Gui::AElement {
  private:
    class Cache {
    private:
      std::map<std::string, SDL_Color> _colors;
      std::map<std::pair<std::string, uint8_t>, TTF_Font*> _fonts;

    public:
      static Cache& getInstance();
      SDL_Color& getColor(std::string const& name);
      TTF_Font *getFont(std::string const& fontType, uint8_t size);

    private:
      Cache();
      ~Cache();
    };

  private:
    std::string const& _content;
    Gui::align _alignment;
    std::string _color;
    std::string _fontFamily;
    uint16_t _size;
    uint16_t _baseX;
    uint16_t _baseY;

  public:
    Text(uint16_t x,
	 uint16_t y,
	 std::string const& content,
	 Gui::align alignment,
	 std::string color,
	 std::string fontFamily = "DejaVuSansMono",
	 uint16_t size = 12);
    ~Text();
    SDL_Surface *render(Ecs::World &world, uint32_t player);
    void setPos(uint16_t x, uint16_t y);

  private:
    void updateSurface();
  };
}
