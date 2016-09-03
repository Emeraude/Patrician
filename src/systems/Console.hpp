#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <thread>
#include "World.hpp"

namespace sys {
  class Console : public Ecs::System::Base {
  private:
    unsigned int _selectedShip;
    std::thread *_thread;
    Ecs::World *_w;
    std::map<std::string, void (sys::Console::*)(std::stringstream&)> _cmds;
    std::map<std::string, Ecs::Entity *(*)(uint16_t, uint16_t)> _types;
    std::map<std::string, unsigned int (*)(Ecs::World &, uint32_t, uint32_t)> _buildingTypes;

    void status(std::stringstream&);
    void help(std::stringstream&);
    void ship(std::stringstream&);
    void city(std::stringstream&);
    void building(std::stringstream&);
    void readCin();
  public:
    Console();
    ~Console();

    void update(Ecs::World &world);
  };
}
