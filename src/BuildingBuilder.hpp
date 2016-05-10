#pragma once

#include "World.hpp"

class BuildingBuilder {
public:
  static Ecs::Entity *office(uint32_t city, uint32_t owner);
};
