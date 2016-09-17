#pragma once

#include "World.hpp"

class BuildingBuilder {
private:
  static Ecs::Entity* base(Ecs::World& w, uint32_t city, uint32_t owner);

public:
  static unsigned int addOffice(Ecs::World& w, uint32_t city, uint32_t owner);

  class Production {
  private:
    static Ecs::Entity* base(Ecs::World& w, uint32_t city, uint32_t owner);

  public:
    static unsigned int addSawmill(Ecs::World& w, uint32_t city, uint32_t owner);
    static unsigned int addBrickworks(Ecs::World& w, uint32_t city, uint32_t owner);
  };
};
