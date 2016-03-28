#pragma once

#include "World.hpp"

namespace System {
  class Move : public Ecs::System::Base {
  public:
    void update(Ecs::World &world);
  };
}
