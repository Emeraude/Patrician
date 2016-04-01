#pragma once

#include <cstdint>
#include "World.hpp"

class ShipBuilder {
private:
  static std::string const randomName();
  static Ecs::Entity *core(uint16_t x, uint16_t y);

public:
  static Ecs::Entity *Snaikka(uint16_t x, uint16_t y);
  static Ecs::Entity *Crayer(uint16_t x, uint16_t y);
};
