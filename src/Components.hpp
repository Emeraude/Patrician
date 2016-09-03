#pragma once

#include <cstdint>
#include <string>
#include "World.hpp"
#include "Resources.hpp"

enum Type {
  SHIP, CITY, BUILDING
};

namespace comp {
  struct Id : Ecs::Component::Base {
    uint32_t value;
    Id(uint32_t v) : value(v) {}
  };

  struct Position : Ecs::Component::Base {
    int16_t x;
    int16_t y;
    Position(int16_t _x, int16_t _y) : x(_x), y(_y) {}
  };

  struct Type : Ecs::Component::Base {
    ::Type type;
    Type(::Type _type) : type(_type) {}
  };

  struct Name : Ecs::Component::Base {
    std::string value;
    Name(std::string const& s) : value(s) {}
  };

  struct Speed : Ecs::Component::Base {
    uint16_t current;
    uint16_t max;
    Speed(uint16_t _max) : current(0), max(_max) {}
  };

  struct Hull : Ecs::Component::Base {
    uint16_t current;
    uint16_t max;
    Hull(uint16_t _max) : current(_max), max(_max) {}
  };

  // This is not a movement vector, but a destination
  struct Move : Ecs::Component::Base {
    uint16_t x;
    uint16_t y;
    Move(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}
  };

  struct Inhabitants : Ecs::Component::Base {
    uint16_t poor;
    uint16_t wealthy;
    uint16_t rich;
    uint16_t beggar;
    Inhabitants(uint16_t _poor, uint16_t _wealthy, uint16_t _rich, uint16_t _beggar) : poor(_poor), wealthy(_wealthy), rich(_rich), beggar(_beggar) {}
    uint16_t total() {
      return poor + wealthy + rich + beggar;
    }
  };

  struct Stock : Ecs::Component::Base {
  private:
    std::vector<uint16_t> _stock;

  public:
    Stock() : _stock(Resource::LENGTH) {}
    uint16_t& at(Resource const r) {
      return _stock.at(r);
    }
  };

  struct Production : Ecs::Component::Base {
  private:
    std::vector<uint16_t> _stock;

  public:
    Production() : _stock(Resource::LENGTH) {}
    uint16_t& at(Resource const r) {
      return _stock.at(r);
    }
  };

  struct Consumption : Ecs::Component::Base {
  private:
    std::vector<uint16_t> _stock;

  public:
    Consumption() : _stock(Resource::LENGTH) {}
    uint16_t& at(Resource const r) {
      return _stock.at(r);
    }
  };

  // Used to know where is a building / where live a trader
  struct City : Ecs::Component::Base {
    uint32_t id;
    City(uint32_t _id) : id(_id) {}
  };

  struct Owner : Ecs::Component::Base {
    uint32_t id;
    Owner(uint32_t _id) : id(_id) {}
  };

  struct Size : Ecs::Component::Base {
    uint32_t value;
    Size(uint32_t _value = std::numeric_limits<uint32_t>::max()) : value(_value) {}
  };

  // Store ids of the buildings
  struct Buildings : Ecs::Component::Base {
    uint32_t office;
  };
}
