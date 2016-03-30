#include <iostream>
#include <string>
#include "Components.hpp"
#include "Console.hpp"
#include "ShipBuilder.hpp"

System::Console::Console() : _thread(new std::thread(&System::Console::readCin, this)), _w(NULL) {
  _types["snaikka"] = &ShipBuilder::Snaikka;
  _types["crayer"] = &ShipBuilder::Crayer;
}

System::Console::~Console() {
  delete _thread;
}

void System::Console::ship(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: ship add <type> <x> <y> | list" << std::endl;
    return;
  }
  std::string cmd;
  ss >> cmd;
  if (cmd == "add") {
    uint16_t x, y;
    std::string type;
    if (!(ss >> type >> x >> y))
      std::cerr << "Usage: ship add <type> <x> <y>" << std::endl;
    else if (_types[type] == NULL)
      std::cerr << "Unknown ship type \"" << type << "\"" << std::endl;
    else
      _w->addEntity(_types[type](x, y));
  }
  else if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<Component::Type>()
	  && it->getComponent<Component::Type>()->type == Type::SHIP)
	std::cout << "\"" << it->getComponent<Component::Name>()->value
		  << "\" in " << it->getComponent<Component::Position>()->x
		  << "," << it->getComponent<Component::Position>()->y << std::endl;
    }
  }
  // Test only; find a proper way to do it
  else if (cmd == "move") {
    for (auto *it: _w->getEntities()) {
      it->addComponent<Component::Move>(500, 600);
      it->getComponent<Component::Speed>()->current = 2;
      break;
    }
  }
}

void System::Console::readCin() {
  std::string in;

  _cmds["ship"] = &System::Console::ship;
  while (true) {
    if (!std::getline(std::cin, in))
      break;
    in.erase(0, in.find_first_not_of(" \n\r\t"));
    in.erase(in.find_last_not_of(" \n\r\t")+1);
    if (in.length() == 0)
      continue;

    std::stringstream ss(in);
    std::string cmd;
    ss >> cmd;
    if (cmd == "exit")
      break;
    else if (_cmds[cmd] != NULL)
      (this->*_cmds[cmd])(ss);
    else
      std::cerr << "Unknown command " << cmd << std::endl;
  }
  _w->stop();
}

void System::Console::update(Ecs::World& w) {
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
