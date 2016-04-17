#pragma once

#include "World.hpp"

namespace sys {
  class Move : public Ecs::System::Base {
  public:
    void update(Ecs::World &world);
  };
}
