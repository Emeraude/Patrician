#pragma once

#include "World.hpp"
#include "Duration.hpp"

namespace sys {
  class Time : public Ecs::System {
  private:
    Duration _date;
    uint32_t _speed;

    void consumption(Ecs::World& w, Ecs::Entity *city);
    void production(Ecs::World& w, Ecs::Entity *building);
  public:
    Time();
    void update(Ecs::World &world);

    Duration const& getDate() const;
    uint32_t const& getSpeed() const;
  };
}
