#pragma once

#include "World.hpp"

namespace System {
  class Time : public Ecs::System::Base {
  private:
    uint32_t _day;

  public:
    Time();
    void update(Ecs::World &world);

    uint32_t const& getDay() const;
  };
}
