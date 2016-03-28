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

    void ship(std::stringstream&);
    void readCin();
  public:
    Console();
    ~Console();

    void update(Ecs::World &world);
  };
}
