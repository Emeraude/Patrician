#include "Sdl.hpp"
#include "Components.hpp"
#include "PlayerBuilder.hpp"
#include "Time.hpp"

sys::Sdl::Sdl() : _pos_x(0), _pos_y(0), _win_w(640), _win_h(480), _player(0) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw Exception();
  if (TTF_Init() == -1)
    throw TTF();
  _window = SDL_CreateWindow("Patrician", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    _win_w, _win_h, SDL_WINDOW_RESIZABLE);
  if (_window == NULL)
    throw Exception();
  _screen = SDL_GetWindowSurface(_window);
  if (_screen == NULL)
    throw Exception();
   _font = TTF_OpenFont("./resources/fonts/DejaVuSansMono.ttf", 12);
  if (_font == NULL)
    throw TTF();
  _sprites["city"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["city"], NULL, SDL_MapRGB(_sprites["city"]->format, 255, 0, 0));
  _sprites["ship"] = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);
  SDL_FillRect(_sprites["ship"], NULL, SDL_MapRGB(_sprites["ship"]->format, 255, 255, 255));
  _hud = new Gui::Hud(640, 480);
}

sys::Sdl::~Sdl() {
  for (auto& it: _sprites) {
    SDL_FreeSurface(it.second);
  }
  delete _hud;
  TTF_CloseFont(_font);
  TTF_Quit();
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
	_hud->updateSize(_win_w, _win_h);
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
  SDL_Color white = {255, 255, 255, 0};

  SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, 0, 0, 0));
  SDL_BlitSurface(_hud->draw(world, _player), NULL, _screen, NULL);
  for (auto *it: world.getEntities()) {
    if (it->getComponent<comp::Type>()->type == Type::CITY) {
      comp::Position *pos = it->getComponent<comp::Position>();
      blitSurface("city", it);
      if (pos->x > _pos_x
	  && pos->y > _pos_y
	  && pos->x < _pos_x + _win_w
	  && pos->y < _pos_y + _win_h) {
	SDL_Surface *msg= TTF_RenderUTF8_Blended(_font, it->getComponent<comp::Name>()->value.c_str(), white);
	SDL_Rect dst;
	dst.x = pos->x - msg->w / 2 - _pos_x;
	dst.y = pos->y - 5 - msg->h - _pos_y;
	SDL_BlitSurface(msg, NULL, _screen, &dst);
	SDL_FreeSurface(msg);
      }
    }
    else if (it->getComponent<comp::Type>()->type == Type::SHIP) {
      blitSurface("ship", it);
    }
  }
  SDL_UpdateWindowSurface(_window);
}

void sys::Sdl::update(Ecs::World &world) {
  if (!_player)
    _player = *::players.begin();
  events(world);
  display(world);
}
