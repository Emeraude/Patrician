#include "Sdl.hpp"
#include "Components.hpp"

sys::Sdl::Sdl() : _pos_x(0), _pos_y(0), _win_w(640), _win_h(480) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw Exception();
  _window = SDL_CreateWindow("Patrician", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    _win_w, _win_h, SDL_WINDOW_RESIZABLE);
  if (_window == NULL)
    throw Exception();
  _screen = SDL_GetWindowSurface(_window);
  if (_screen == NULL)
    throw Exception();
  _sprites["city"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["city"], NULL, SDL_MapRGB(_sprites["city"]->format, 255, 0, 0));
  _sprites["ship"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["ship"], NULL, SDL_MapRGB(_sprites["ship"]->format, 255, 255, 255));
}

sys::Sdl::~Sdl() {
  for (auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
  SDL_DestroyWindow(_window);
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
    	_pos_y -=10;
    	break;
      case SDLK_DOWN:
    	_pos_y += 10;
    	break;
      case SDLK_RIGHT:
    	_pos_x += 10;
    	break;
      case SDLK_LEFT:
    	_pos_x -= 10;
      }
    }
    else if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
	  || event.window.event == SDL_WINDOWEVENT_RESIZED) {
	_win_w = event.window.data1;
	_win_h = event.window.data2;
	_screen = SDL_GetWindowSurface(_window);
	if (_screen == NULL)
	  throw Exception();
      }
    }
  }
  if (_pos_x > 1000 - _win_w)
    _pos_x = 1000 - _win_w;
  if (_pos_x < 0)
    _pos_x = 0;
  if (_pos_y > 1000 - _win_h)
    _pos_y = 1000 - _win_h;
  if (_pos_y < 0)
    _pos_y = 0;
}

// Returns true if the surface has be drawn (within the window), false otherwise
bool sys::Sdl::blitSurface(std::string const& name, Ecs::Entity *e) {
  SDL_Rect dst;
  comp::Position *pos = e->getComponent<comp::Position>();

  if (pos->x > _pos_x
      && pos->y > _pos_y
      && pos->x < _pos_x + _win_w
      && pos->y < _pos_y + _win_h) {
    dst.x = pos->x - _pos_x;
    dst.y = pos->y - _pos_y;
    SDL_BlitSurface(_sprites[name], NULL, _screen, &dst);
    return true;
  }
  return false;
}

void sys::Sdl::display(Ecs::World &world) {
  SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, 0, 0, 0));
  for (auto *it: world.getEntities()) {
    if (it->getComponent<comp::Type>()->type == Type::CITY) {
      blitSurface("city", it);
    }
    else if (it->getComponent<comp::Type>()->type == Type::SHIP) {
      blitSurface("ship", it);
    }
  }
  SDL_UpdateWindowSurface(_window);
}

void sys::Sdl::update(Ecs::World &world) {
  events(world);
  display(world);
}
