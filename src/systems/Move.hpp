#pragma once

#include "World.hpp"

namespace sys {
  class Move : public Ecs::System {
  private:
    void checkCities(Ecs::World &w, Ecs::Entity *e);

  public:
    void update(Ecs::World &world);
  };
}
