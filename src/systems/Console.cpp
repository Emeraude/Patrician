#include <iostream>
#include <string>
#include "Components.hpp"
#include "Console.hpp"
#include "ShipBuilder.hpp"
#include "systems/Time.hpp"

System::Console::Console() : _thread(new std::thread(&System::Console::readCin, this)), _w(NULL) {
  _types["snaikka"] = &ShipBuilder::Snaikka;
  _types["crayer"] = &ShipBuilder::Crayer;
}

System::Console::~Console() {
  delete _thread;
}

void System::Console::help(std::stringstream&) {
  std::cout << "Available commands:" << std::endl
	    << "\tcity details <city> | list | stock <city>" << std::endl
	    << "\thelp" << std::endl
	    << "\tship add <type> <x> <y> | list" << std::endl
	    << "\tstatus" << std::endl;
}

void System::Console::status(std::stringstream&) {
  std::cout << "Day " << _w->getSystem<System::Time>()->getDay() << std::endl;
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
    else {
      Ecs::Entity *e = _types[type](x, y);
      unsigned int id = _w->addEntity(e);
      e->addComponent<Component::Id>(id);
    }
  }
  else if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<Component::Type>()
	  && it->getComponent<Component::Type>()->type == Type::SHIP)
	std::cout << "#" << it->getComponent<Component::Id>()->value
		  << " \"" << it->getComponent<Component::Name>()->value
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

void System::Console::city(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: city details <city> | list | stock <city>" << std::endl;
    return;
  }
  std::string cmd;
  ss >> cmd;
  if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<Component::Type>()
	  && it->getComponent<Component::Type>()->type == Type::CITY)
	std::cout << "\"" << it->getComponent<Component::Name>()->value
		  << "\" (" << it->getComponent<Component::Inhabitants>()->total()
		  << ") in " << it->getComponent<Component::Position>()->x
		  << "," << it->getComponent<Component::Position>()->y << std::endl;
    }
  }
  else if (cmd == "details") {
    std::string cityName;
    if (!(ss >> cityName))
      std::cerr << "Usage: city details <city>" << std::endl;
    else {
      // Very inefficient, we have to do O(log(n)) instead of O(n)
      for (auto *it: _w->getEntities()) {
	if (it->hasComponent<Component::Type>()
	    && it->getComponent<Component::Type>()->type == Type::CITY
	    && it->hasComponent<Component::Name>()
	    && it->getComponent<Component::Name>()->value == cityName)
	  std::cout << "\"" << cityName
		    << "\" in " << it->getComponent<Component::Position>()->x
		    << "," << it->getComponent<Component::Position>()->y
		    << ": " << it->getComponent<Component::Inhabitants>()->poor
		    << " poors, " << it->getComponent<Component::Inhabitants>()->wealthy
		    << " wealthy, " << it->getComponent<Component::Inhabitants>()->rich
		    << " rich, " << it->getComponent<Component::Inhabitants>()->beggar
		    << " beggars" << std::endl;
      }
    }
  }
  else if (cmd == "stock") {
    std::string cityName;
    if (!(ss >> cityName))
      std::cerr << "Usage: city details <city>" << std::endl;
    else {
      // very inefficient too
      for (auto *it: _w->getEntities()) {
	if (it->hasComponent<Component::Type>()
	    && it->getComponent<Component::Type>()->type == Type::CITY
	    && it->hasComponent<Component::Name>()
	    && it->getComponent<Component::Name>()->value == cityName) {
	  Component::Stock s = *it->getComponent<Component::Stock>();
	  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i)
	    std::cout << infosResource[i].name << " " << s.at(static_cast<Resource>(i)) << std::endl;
	}
      }
    }
  }
  else {
    std::cerr << "Unknown command \"city " << cmd << "\"" << std::endl;
  }
}

void System::Console::readCin() {
  std::string in;

  _cmds["ship"] = &System::Console::ship;
  _cmds["city"] = &System::Console::city;
  _cmds["help"] = &System::Console::help;
  _cmds["status"] = &System::Console::status;
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, in)) {
      std::cout << "exit" << std::endl;
      break;
    }
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
