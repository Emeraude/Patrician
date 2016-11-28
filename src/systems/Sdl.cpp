#include "Sdl.hpp"
#include "Components.hpp"

sys::Sdl::Sdl() : pos_x(0), pos_y(0), win_w(640), win_h(480) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw Exception();
  window = SDL_CreateWindow("Patrician", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    win_w, win_h, 0);
  if (window == NULL)
    throw Exception();
  screen = SDL_GetWindowSurface(window);
  if (screen == NULL)
    throw Exception();
}

sys::Sdl::~Sdl() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void sys::Sdl::events(Ecs::World &world) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      world.stop();
    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_UP:
    	pos_y -=10;
    	break;
      case SDLK_DOWN:
    	pos_y += 10;
    	break;
      case SDLK_RIGHT:
    	pos_x += 10;
    	break;
      case SDLK_LEFT:
    	pos_x -= 10;
      }
    }
  }
  if (pos_x > 1000 - win_w)
    pos_x = 1000 - win_w;
  if (pos_x < 0)
    pos_x = 0;
  if (pos_y > 1000 - win_h)
    pos_y = 1000 - win_h;
  if (pos_y < 0)
    pos_y = 0;
}

void sys::Sdl::display(Ecs::World &world) {
  SDL_Surface *city, *ship;
  SDL_Rect dst;
  city = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  ship = SDL_CreateRGBSurface(0, 5, 5, 32, 0, 0, 0, 0);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  SDL_FillRect(city, NULL, SDL_MapRGB(city->format, 255, 0, 0));
  SDL_FillRect(ship, NULL, SDL_MapRGB(ship->format, 255, 255, 255));
  for (auto *it: world.getEntities()) {
    if (it->getComponent<comp::Type>()->type == Type::CITY) {
      comp::Position *pos = it->getComponent<comp::Position>();
      if (pos->x > pos_x && pos->y > pos_y
	  && pos->x < pos_x + win_w
	  && pos->y < pos_y + win_h) {
	dst.x = pos->x - pos_x;
	dst.y = pos->y - pos_y;
	SDL_BlitSurface(city, NULL, screen, &dst);
      }
    }
    else if (it->getComponent<comp::Type>()->type == Type::SHIP) {
      comp::Position *pos = it->getComponent<comp::Position>();
      if (pos->x > pos_x && pos->y > pos_y
	  && pos->x < pos_x + win_w
	  && pos->y < pos_y + win_h) {
	dst.x = pos->x - pos_x;
	dst.y = pos->y - pos_y;
	SDL_BlitSurface(ship, NULL, screen, &dst);
      }
    }
  }
  SDL_FreeSurface(city);
  SDL_FreeSurface(ship);
  SDL_UpdateWindowSurface(window);
}

void sys::Sdl::update(Ecs::World &world) {
  events(world);
  display(world);
}
