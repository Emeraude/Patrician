#pragma once

#include <cstdint>
#include "World.hpp"

enum Type {
  SHIP, CITY, BUILDING
};

namespace Component {
  struct Position : Ecs::Component::Base {
    int16_t x;
    int16_t y;
    Position(int16_t _x, int16_t _y) : x(_x), y(_y) {}
  };

  struct Type : Ecs::Component::Base {
    ::Type type;
    Type(::Type _type) : type(_type) {}
  };
}
