#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <thread>
#include "World.hpp"
#include "src/Exceptions.hpp"

namespace sys {
  class Console : public Ecs::System::Base {
  private:
    struct Cmd {
      void (sys::Console::*ft)(std::stringstream&);
      std::string usage;
    };

    unsigned int _selectedShip;
    unsigned int _player;
    std::thread *_thread;
    Ecs::World *_w;
    std::map<std::string, Cmd> _cmds;
    std::map<std::string, Ecs::Entity *(*)(Ecs::World &, uint32_t, uint16_t, uint16_t)> _types;
    std::map<std::string, unsigned int (*)(Ecs::World &, uint32_t, uint32_t)> _buildingTypes;

    void status(std::stringstream&);
    void help(std::stringstream&);
    void ship(std::stringstream&);
    void buy(std::stringstream&);
    void sell(std::stringstream&);
    void city(std::stringstream&);
    void building(std::stringstream&);
    void readCin();

    void checkSelectedShip();
    void checkResourceType(std::string const& str);
  public:
    Console();
    ~Console();

    void update(Ecs::World &world);

  private:
    class Exception {
    public:
      class Usage : public ::Exception::Base {
      public:
	Usage(std::string const& str) : ::Exception::Base(str) {}
      };
    };
  };
}
