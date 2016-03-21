#include <iostream>
#include <string>
#include "Components.hpp"
#include "console.hpp"

System::Console::Console() : _thread(new std::thread(&System::Console::readCin, this)), _w(NULL) {}

System::Console::~Console() {
  delete _thread;
}

void System::Console::ship(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: ship add <x> <y> | list" << std::endl;
    return;
  }
  std::string cmd;
  ss >> cmd;
  if (cmd == "add") {
    Ecs::Entity *e = new Ecs::Entity();
    int x, y;
    // TODO : raise error if an argument is missing
    ss >> x >> y;
    e->addComponent<Component::Position>(x, y);
    e->addComponent<Component::Type>(Type::SHIP);
    _w->addEntity(e);
  }
  else if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<Component::Type>()
	  && it->getComponent<Component::Type>()->type == Type::SHIP)
	std::cout << "A ship in " << it->getComponent<Component::Position>()->x <<
	  "," << it->getComponent<Component::Position>()->y << std::endl;
    }
  }
}

void System::Console::readCin() {
  std::string in;

  _cmds["ship"] = &System::Console::ship;
  while(true) {
    if (!std::getline(std::cin, in)) {
      _w->stop();
      return;
    }
    in.erase(0, in.find_first_not_of(" \n\r\t"));
    in.erase(in.find_last_not_of(" \n\r\t")+1);
    if (in.length() == 0)
      continue;

    std::stringstream ss(in);
    std::string cmd;
    ss >> cmd;
    if (cmd == "exit") {
      _w->stop();
      return;
    }
    else if (_cmds[cmd] != NULL)
      (this->*_cmds[cmd])(ss);
    else
      std::cerr << "Unknown command " << cmd << std::endl;
  }
}

void System::Console::update(Ecs::World& w) {
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
