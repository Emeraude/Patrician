#pragma once

#include <cstdint>
#include "World.hpp"

class ShipBuilder {
private:
  static std::string const randomName();

public:
  static Ecs::Entity *Snaikka(uint16_t x, uint16_t y);
  static Ecs::Entity *Crayer(uint16_t x, uint16_t y);
};
