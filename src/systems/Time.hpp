#pragma once

#include "World.hpp"

namespace sys {
  class Time : public Ecs::System {
  private:
    uint32_t _day;
    uint32_t _speed;

    void consumption(Ecs::World& w, Ecs::Entity *city);
    void production(Ecs::World& w, Ecs::Entity *building);
  public:
    Time();
    void update(Ecs::World &world);

    uint32_t const& getDay() const;
    uint32_t const& getSpeed() const;
  };
}
