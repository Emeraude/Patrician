#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "World.hpp"
#include "Components.hpp"
#include "src/Exceptions.hpp"
#include "gui/Game.hpp"
#include "gui/Hud.hpp"
#include "gui/Popin.hpp"

namespace sys {
  class Sdl : public Ecs::System::Base {
  private:
    SDL_Window *_window;
    SDL_Surface *_screen;
    int _win_w;
    int _win_h;
    int _velocity_x;
    int _velocity_y;
    unsigned int _player;
    Gui::Hud *_hud;
    Gui::Game *_game;
    Gui::Popin *_popin;
    Ecs::Entity *_selected;

    void events(Ecs::World &world);
    void display(Ecs::World &world);

  public:
    Sdl();
    ~Sdl();
    void update(Ecs::World &world);
  };
}
