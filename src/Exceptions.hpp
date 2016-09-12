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
}
