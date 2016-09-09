#include <iostream>
#include <string>
#include "Components.hpp"
#include "Console.hpp"
#include "ShipBuilder.hpp"
#include "CityBuilder.hpp"
#include "BuildingBuilder.hpp"
#include "systems/Time.hpp"

sys::Console::Console() : _selectedShip(0), _thread(new std::thread(&sys::Console::readCin, this)), _w(NULL) {
  _types["snaikka"] = &ShipBuilder::Snaikka;
  _types["crayer"] = &ShipBuilder::Crayer;

  _buildingTypes["sawmill"] = &BuildingBuilder::Production::addSawmill;
}

sys::Console::~Console() {
  delete _thread;
}

void sys::Console::help(std::stringstream&) {
  std::cout << "Available commands:" << std::endl
	    << "\tbuilding create <city> <type> | list <city>" << std::endl
	    << "\tcity details <city> | list | stock <city>" << std::endl
	    << "\thelp" << std::endl
	    << "\tship add <type> <x> <y> | list | (<city> | <x> <y>) | select <id>" << std::endl
	    << "\tstatus" << std::endl;
}

void sys::Console::status(std::stringstream&) {
  sys::Time& sys = *_w->getSystem<sys::Time>();

  std::cout << "Day " << sys.getDay() << std::endl
	    << "Speed: 1 day ~= " << (sys.getSpeed() / 100.0) << " seconds" << std::endl;
}

void sys::Console::ship(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: ship add <type> <x> <y> | list | move (<city> | <x> <y>) | select <id>" << std::endl;
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
      e->addComponent<comp::Id>(id);
    }
  }
  else if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<comp::Type>()
	  && it->getComponent<comp::Type>()->type == Type::SHIP)
	std::cout << "#" << it->getComponent<comp::Id>()->value
		  << " \"" << it->getComponent<comp::Name>()->value
		  << "\" in " << it->getComponent<comp::Position>()->x
		  << "," << it->getComponent<comp::Position>()->y << std::endl;
    }
  }
  else if (cmd == "move") {
    uint16_t x, y;
    std::string cityName;
    if (ss >> cityName) {
      if (ss >> y) {
	try {
	  x = std::stoi(cityName);
	} catch (...) {
	  std::cerr << "Usage: ship move (<city> | <x> <y>)" << std::endl;
	  return;
	}
      }
      else {
	try {
	  Ecs::Entity *e = _w->getEntities()[::cityNames.at(cityName)];
	  x = e->getComponent<comp::Position>()->x;
	  y = e->getComponent<comp::Position>()->y;
	} catch (std::out_of_range&) {
	  std::cerr << "No city \"" << cityName << "\" found" << std::endl;
	  return;
	}
      }
      try {
	Ecs::Entity& e = *_w->getEntities().at(_selectedShip);
	if (e.hasComponent<comp::Type>()
	    && e.getComponent<comp::Type>()->type != Type::SHIP)
	  throw std::out_of_range("No ship selected");
	e.addComponent<comp::Move>(x, y);
	e.getComponent<comp::Speed>()->current = e.getComponent<comp::Speed>()->max;
      }
      catch (std::out_of_range&) {
	std::cerr << "No ship selected" << std::endl;
      }
    }
    else
      std::cerr << "Usage: ship move (<city> | <x> <y>)" << std::endl;
  }
  else if (cmd == "select") {
    unsigned int id;
    if (!(ss >> id))
      std::cerr << "Usage: ship select <id>" << std::endl;
    else {
      try {
	Ecs::Entity& e = *_w->getEntities().at(id);
	if (e.hasComponent<comp::Type>()
	    && e.getComponent<comp::Type>()->type != Type::SHIP)
	  throw std::out_of_range("Not a ship");
	_selectedShip = id;
      } catch (std::out_of_range&) {
	std::cerr << "There is no ship with id #" << id << std::endl;
      }
    }
  }
}

void sys::Console::city(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: city details <city> | list stock <city>" << std::endl;
    return;
  }
  std::string cmd;
  ss >> cmd;
  if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->hasComponent<comp::Type>()
	  && it->getComponent<comp::Type>()->type == Type::CITY)
	std::cout << "\"" << it->getComponent<comp::Name>()->value
		  << "\" (" << it->getComponent<comp::Inhabitants>()->total()
		  << ") in " << it->getComponent<comp::Position>()->x
		  << "," << it->getComponent<comp::Position>()->y << std::endl;
    }
  }
  else if (cmd == "details") {
    std::string cityName;
    if (!(ss >> cityName))
      std::cerr << "Usage: city details <city>" << std::endl;
    else {
      try {
	Ecs::Entity *e = _w->getEntities()[::cityNames.at(cityName)];
	std::cout << "\"" << cityName
		  << "\" in " << e->getComponent<comp::Position>()->x
		  << "," << e->getComponent<comp::Position>()->y
		  << ": " << e->getComponent<comp::Inhabitants>()->poor
		  << " poors, " << e->getComponent<comp::Inhabitants>()->wealthy
		  << " wealthy, " << e->getComponent<comp::Inhabitants>()->rich
		  << " rich, " << e->getComponent<comp::Inhabitants>()->beggar
		  << " beggars" << std::endl;
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
    }
  }
  else if (cmd == "stock") {
    std::string cityName;
    if (!(ss >> cityName))
      std::cerr << "Usage: city stock <city>" << std::endl;
    else {
      try {
	Ecs::Entity *city = _w->getEntity(::cityNames.at(cityName));
	Ecs::Entity *e = _w->getEntity(city->getComponent<comp::Buildings>()->office);
	comp::Stock *s = e->getComponent<comp::Stock>();
	for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i)
	  std::cout << infosResource[i].name << " " << s->at(static_cast<Resource>(i)) << std::endl;
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
    }
  }
  else {
    std::cerr << "Unknown command \"city " << cmd << "\"" << std::endl;
  }
}

void sys::Console::building(std::stringstream& ss) {
  if (ss.eof()) {
    std::cout << "Usage: building create <city> <type> | list <city>" << std::endl;
    return;
  }
  std::string cmd;
  ss >> cmd;
  if (cmd == "create") {
    std::string cityName, type;
    if (!(ss >> cityName >> type))
      std::cerr << "Usage: building create <city> <type>" << std::endl;
    else if (_buildingTypes[type] == NULL)
      std::cerr << "Unknown building type \"" << type << "\"" << std::endl;
    else
      try {
	// TODO: replace by real player id
	_buildingTypes[type](*_w, ::cityNames.at(cityName), 1);
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
  }
  else if (cmd == "list") {
    std::string cityName;
    if (!(ss >> cityName))
      std::cerr << "Usage: building list <city>" << std::endl;
    else
      try {
	for (auto *it: _w->getEntities()) {
	  if (it->hasComponent<comp::Type>()
	      && it->hasComponent<comp::City>()
	      && it->getComponent<comp::Type>()->type == Type::BUILDING
	      && it->getComponent<comp::City>()->id == ::cityNames.at(cityName)) {
	    if (it->hasComponent<comp::Name>())
	      std::cout << it->getComponent<comp::Name>()->value << std::endl;
	    else
	      std::cout << "Unknown building" << std::endl;
	  }
	}
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
  }
}

void sys::Console::readCin() {
  std::string in;

  _cmds["ship"] = &sys::Console::ship;
  _cmds["building"] = &sys::Console::building;
  _cmds["city"] = &sys::Console::city;
  _cmds["help"] = &sys::Console::help;
  _cmds["status"] = &sys::Console::status;
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

void sys::Console::update(Ecs::World& w) {
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
