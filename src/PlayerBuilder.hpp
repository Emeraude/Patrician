#pragma once

#include <cstdint>
#include <vector>
#include "World.hpp"

extern std::vector<unsigned int> players;

class PlayerBuilder {
public:
  static Ecs::Entity *addPlayer(Ecs::World& w, uint32_t city);
};
