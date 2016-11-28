#pragma once

#include <string>
#include <map>
#include <SDL.h>
#include "World.hpp"
#include "src/Exceptions.hpp"

namespace sys {
  class Sdl : public Ecs::System::Base {
  private:
    std::map<std::string, SDL_Surface *> _sprites;
    SDL_Window *_window;
    SDL_Surface *_screen;
    int _pos_x;
    int _pos_y;
    int _win_w;
    int _win_h;

  public:
    Sdl();
    ~Sdl();
    void events(Ecs::World &world);
    bool blitSurface(std::string const& name, Ecs::Entity *e);
    void display(Ecs::World &world);
    void update(Ecs::World &world);

  private:
    class Exception : public ::Exception::Base {
    public:
      Exception() : ::Exception::Base(std::string("SDL error: ") + SDL_GetError()) {}
    };
  };
}
