#pragma once

#include <cstdint>
#include "World.hpp"

class ShipBuilder {
private:
  static std::string const randomName();
  static Ecs::Entity *core(Ecs::World& w, uint16_t x, uint16_t y);

public:
  static Ecs::Entity *addSnaikka(Ecs::World& w, uint16_t x, uint16_t y);
  static Ecs::Entity *addCrayer(Ecs::World& w, uint16_t x, uint16_t y);
};
