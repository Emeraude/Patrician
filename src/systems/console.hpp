#pragma once

#include <thread>
#include "World.hpp"

namespace System {
  class Console : public Ecs::System::Base {
  private:
    std::thread *_thread;
    Ecs::World *_w;

  public:
    Console();
    ~Console();

    void readCin();
    void update(Ecs::World &world);
  };
}
