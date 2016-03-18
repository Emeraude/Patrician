#pragma once

#include <map>
#include <string>
#include <thread>
#include "World.hpp"

namespace System {
  class Console : public Ecs::System::Base {
  private:
    std::thread *_thread;
    Ecs::World *_w;
    std::map<std::string, void (System::Console::*)(std::stringstream&)> _cmds;

  public:
    Console();
    ~Console();

    void readCin();
    void update(Ecs::World &world);
  };
}
