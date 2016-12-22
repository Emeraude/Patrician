#include <SDL.h>
#include <SDL_ttf.h>
#include "Exceptions.hpp"

::Exception::Base::Base(std::string const& str) : _str(str) {}

::Exception::Base::~Base() {}

const char *::Exception::Base::what() const throw() {
  return _str.c_str();
}

::Exception::Ship::Ship(std::string const& str) : Exception::Base(str) {}

::Exception::Resource::Resource(std::string const& str) : Exception::Base(str) {}

::Exception::Building::Building(std::string const& str) : Exception::Base(str) {}

::Exception::Sdl::Sdl(std::string const& str) : Exception::Base(std::string("SDL error: ") + str + " " + SDL_GetError()) {}

::Exception::Ttf::Ttf(std::string const& str) : Exception::Base(std::string("TTF error: ") + str + " " + TTF_GetError()) {}
