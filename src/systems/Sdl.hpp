#pragma once

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"
#include "src/Exceptions.hpp"

namespace sys {
  class Sdl : public Ecs::System::Base {
  private:
    std::map<std::string, SDL_Surface *> _sprites;
    SDL_Window *_window;
    SDL_Surface *_screen;
    TTF_Font *_font;
    int _pos_x;
    int _pos_y;
    int _win_w;
    int _win_h;
    unsigned int _player;

    void events(Ecs::World &world);
    bool blitSurface(std::string const& name, Ecs::Entity *e);
    void printUI(Ecs::World &world);
    void display(Ecs::World &world);

  public:
    Sdl();
    ~Sdl();
    void update(Ecs::World &world);

  private:
    class Exception : public ::Exception::Base {
    public:
      Exception() : ::Exception::Base(std::string("SDL error: ") + SDL_GetError()) {}
    };

    class TTF : public ::Exception::Base {
    public:
      TTF() : ::Exception::Base(std::string("TTF error: ") + TTF_GetError()) {}
    };
  };
}
