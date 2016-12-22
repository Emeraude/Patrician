#pragma once

#include <exception>
#include <string>

namespace Exception {
  class Base : public std::exception {
  private:
    std::string const _str;

  public:
    Base(std::string const& str);
    ~Base();
    const char *what() const throw();
  };

  class Ship : public ::Exception::Base {
  public:
    Ship(std::string const& str);
  };

  class Resource : public ::Exception::Base {
  public:
    Resource(std::string const& str);
  };

  class Building : public ::Exception::Base {
  public:
    Building(std::string const& str);
  };

  class Sdl : public ::Exception::Base {
  public:
    Sdl(std::string const& str = "");
  };

  class Ttf : public ::Exception::Base {
  public:
    Ttf(std::string const& str = "");
  };
}
