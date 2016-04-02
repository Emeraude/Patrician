#pragma once

#include <cstdint>
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
    std::map<std::string, Ecs::Entity *(*)(uint16_t, uint16_t)> _types;

    void help(std::stringstream&);
    void ship(std::stringstream&);
    void city(std::stringstream&);
    void readCin();
  public:
    Console();
    ~Console();

    void update(Ecs::World &world);
  };
}
