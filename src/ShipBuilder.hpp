#pragma once

#include <cstdint>
#include "World.hpp"

class ShipBuilder {
private:
  static std::string const randomName();

public:
  static Ecs::Entity *Snaikka(int16_t x, int16_t y);
  static Ecs::Entity *Crayer(int16_t x, int16_t y);
};
