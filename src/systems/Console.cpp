#include <iostream>
#include <string>
#include "Components.hpp"
#include "Console.hpp"
#include "ShipBuilder.hpp"
#include "CityBuilder.hpp"
#include "BuildingBuilder.hpp"
#include "PlayerBuilder.hpp"
#include "systems/Time.hpp"

sys::Console::Console() : _selectedShip(0), _player(0), _thread(new std::thread(&sys::Console::readCin, this)), _w(NULL) {
  _types["snaikka"] = &ShipBuilder::addSnaikka;
  _types["crayer"] = &ShipBuilder::addCrayer;

  _buildingTypes["sawmill"] = &BuildingBuilder::Production::addSawmill;
  _buildingTypes["brickworks"] = &BuildingBuilder::Production::addBrickworks;
}

sys::Console::~Console() {
  delete _thread;
}

void sys::Console::help(std::stringstream&) {
  std::cout << "Available commands:" << std::endl;
  for (auto& it : _cmds)
    std::cout << "\t" << it.second.usage << std::endl;
}

void sys::Console::status(std::stringstream&) {
  sys::Time& sys = *_w->get<sys::Time>();

  std::cout << "Player #" << _player << std::endl
	    << "Date: " << sys.getDate().toString() << std::endl
	    << "Speed: 1 day ~= " << (sys.getSpeed() * 1.440) << " seconds" << std::endl;
}

void sys::Console::ship(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string cmd;
  ss >> cmd;
  if (cmd == "add") {
    uint16_t x, y;
    std::string type;
    if (!(ss >> type >> x >> y))
      throw Exception::Usage("");
    else if (_types[type] == NULL)
      std::cerr << "Unknown ship type \"" << type << "\"" << std::endl;
    else
      _types[type](*_w, _player, x, y);
  }
  else if (cmd == "list") {
    for (auto *it: _w->getEntities()) {
      if (it->has<comp::Type>()
	  && it->get<comp::Type>()->type == Type::SHIP)
	std::cout << "#" << it->get<comp::Id>()->value
		  << " \"" << it->get<comp::Name>()->value
		  << "\" in " << it->get<comp::Position>()->x
		  << "," << it->get<comp::Position>()->y << std::endl;
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
	  throw Exception::Usage("");
	}
      }
      else {
	try {
	  Ecs::Entity *e = _w->getEntities()[::cityNames.at(cityName)];
	  x = e->get<comp::Position>()->x;
	  y = e->get<comp::Position>()->y;
	} catch (std::out_of_range&) {
	  std::cerr << "No city \"" << cityName << "\" found" << std::endl;
	  return;
	}
      }
      this->checkSelectedShip();
      Ecs::Entity& e = *_w->getEntities().at(_selectedShip);
      e.add<comp::Move>(x, y);
      e.get<comp::Speed>()->current = e.get<comp::Speed>()->max;
    }
    else
      throw Exception::Usage("");
  }
  else if (cmd == "select") {
    unsigned int id;
    if (!(ss >> id))
      throw Exception::Usage("");
    else {
      try {
	Ecs::Entity& e = *_w->getEntities().at(id);
	if (e.has<comp::Type>()
	    && e.get<comp::Type>()->type != Type::SHIP)
	  throw std::out_of_range("Not a ship");
	_selectedShip = id;
      } catch (std::out_of_range&) {
      	std::cerr << "There is no ship with id #" << id << std::endl;
      }
    }
  }
  else if (cmd == "details") {
    Ecs::Entity& e = *_w->getEntities().at(_selectedShip);
    this->checkSelectedShip();
    std::cout << "Ship \"" << e.get<comp::Name>()->value << "\" #" << _selectedShip
	      << " in " << e.get<comp::Position>()->x
	      << "," << e.get<comp::Position>()->y;
    if (e.has<comp::City>())
      std::cout << " (" << _w->get(e.get<comp::City>()->id)->get<comp::Name>()->value << ")";
    std::cout << std::endl;
    comp::Stock *s = e.get<comp::Stock>();
    for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
      if (s->at(static_cast<Resource>(i)).quantity > 0)
      std::cout << infosResource[i].name << " " << s->at(static_cast<Resource>(i)).quantity << std::endl;
    }
  }
}

void sys::Console::city(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string cmd;
  ss >> cmd;
  if (cmd == "list") {
    for (auto& it: ::cityNames) {
      Ecs::Entity *city = _w->get(it.second);
      std::cout << "\"" << city->get<comp::Name>()->value
		<< "\" (" << city->get<comp::Inhabitants>()->total()
		<< ") in " << city->get<comp::Position>()->x
		<< "," << city->get<comp::Position>()->y << std::endl;
    }
  }
  else if (cmd == "details") {
    std::string cityName;
    if (!(ss >> cityName))
      throw Exception::Usage("");
    else {
      try {
	Ecs::Entity *e = _w->getEntities()[::cityNames.at(cityName)];
	std::cout << "\"" << cityName
		  << "\" in " << e->get<comp::Position>()->x
		  << "," << e->get<comp::Position>()->y
		  << ": " << e->get<comp::Inhabitants>()->poor
		  << " poors, " << e->get<comp::Inhabitants>()->wealthy
		  << " wealthy, " << e->get<comp::Inhabitants>()->rich
		  << " rich, " << e->get<comp::Inhabitants>()->beggar
		  << " beggars" << std::endl;
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
    }
  }
  else if (cmd == "stock") {
    std::string cityName;
    if (!(ss >> cityName))
      throw Exception::Usage("");
    else {
      try {
	Ecs::Entity *city = _w->get(::cityNames.at(cityName));
	Ecs::Entity *e = _w->get(city->get<comp::Buildings>()->office);
	comp::Stock *s = e->get<comp::Stock>();
	for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i)
	  std::cout << infosResource[i].name << " " << s->at(static_cast<Resource>(i)).quantity << std::endl;
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
    }
  }
  else
    throw Exception::Usage("");
}

void sys::Console::building(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string cmd;
  ss >> cmd;
  if (cmd == "create") {
    std::string cityName, type;
    if (!(ss >> cityName >> type))
      throw Exception::Usage("");
    else if (_buildingTypes[type] == NULL)
      std::cerr << "Unknown building type \"" << type << "\"" << std::endl;
    else
      try {
	_buildingTypes[type](*_w, ::cityNames.at(cityName), _player);
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
  }
  else if (cmd == "list") {
    std::string cityName;
    if (!(ss >> cityName))
      throw Exception::Usage("");
    else
      try {
	for (auto *it: _w->getEntities()) {
	  if (it->has<comp::Type>()
	      && it->has<comp::City>()
	      && it->get<comp::Type>()->type == Type::BUILDING
	      && it->get<comp::City>()->id == ::cityNames.at(cityName)) {
	    if (it->has<comp::Name>())
	      std::cout << it->get<comp::Name>()->value << std::endl;
	    else
	      std::cout << "Unknown building" << std::endl;
	  }
	}
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
  }
}

void sys::Console::buy(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string resourceStr;
  uint32_t quantity;
  if (!(ss >> resourceStr >> quantity))
    throw Exception::Usage("");
  this->checkResourceType(resourceStr);
  this->checkSelectedShip();
  Ecs::Entity *ship = _w->get(_selectedShip);
  Resource resource = resourceNames.at(resourceStr);
  if (!ship->has<comp::City>()) {
    std::cerr << "Ship #" << _selectedShip << " is not in a city" << std::endl;
    return;
  }
  Ecs::Entity *city = _w->get(ship->get<comp::City>()->id);
  Ecs::Entity *office = _w->get(city->get<comp::Buildings>()->office);
  Ecs::Entity *player = _w->get(_player);
  comp::Stock *cityStock = office->get<comp::Stock>();
  unsigned int price = infosResource[resource].cost * quantity;
  if (cityStock->at(resource).quantity < quantity) {
    std::cerr << "City only have " << cityStock->at(resource).quantity
	      << " " << resourceStr << std::endl;
  }
  else if (player->get<comp::Money>()->value < price) {
    std::cerr << "You don't have enough money. It costs " << price
	      << " and you have " << player->get<comp::Money>()->value << std::endl;
  }
  else {
    player->get<comp::Money>()->value -= price;
    cityStock->at(resource).quantity -= quantity;
    ship->get<comp::Stock>()->at(resource).quantity += quantity;
  }
}

void sys::Console::checkSelectedShip() {
  try {
    Ecs::Entity *ship = _w->get(_selectedShip);
    if (!ship->has<comp::Type>()
	|| ship->get<comp::Type>()->type != Type::SHIP)
      throw std::out_of_range("");
  } catch (std::out_of_range&) {
    throw ::Exception::Ship("No ship selected");
  }
}

void sys::Console::checkResourceType(std::string const& str) {
  try {
    resourceNames.at(str);
  } catch (std::out_of_range const&) {
    std::ostringstream oss("");
    oss << "Resource \"" << str << "\" does not exist";
    throw ::Exception::Resource(oss.str());
  }
}

void sys::Console::sell(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string resourceStr;
  uint32_t quantity;
  if (!(ss >> resourceStr >> quantity))
    throw Exception::Usage("");
  this->checkResourceType(resourceStr);
  this->checkSelectedShip();
  Ecs::Entity *ship = _w->get(_selectedShip);
  Resource resource = resourceNames.at(resourceStr);
  if (!ship->has<comp::City>()) {
    std::cerr << "Ship #" << _selectedShip << " is not in a city" << std::endl;
    return;
  }
  Ecs::Entity *city = _w->get(ship->get<comp::City>()->id);
  Ecs::Entity *office = _w->get(city->get<comp::Buildings>()->office);
  Ecs::Entity *player = _w->get(_player);
  comp::Stock *cityStock = office->get<comp::Stock>();
  unsigned int price = infosResource[resource].cost * quantity;
  if (ship->get<comp::Stock>()->at(resource).quantity < quantity) {
    std::cerr << "Your ship only have " << ship->get<comp::Stock>()->at(resource).quantity
	      << " " << resourceStr << std::endl;
  }
  else {
    player->get<comp::Money>()->value += price;
    cityStock->at(resource).quantity += quantity;
    ship->get<comp::Stock>()->at(resource).quantity -= quantity;
  }
}

void sys::Console::readCin() {
  std::string in;

  _cmds["building"] = {&sys::Console::building, "building create <city> <type> | list <city>"};
  _cmds["buy"] = {&sys::Console::buy, "buy <resource> <quantity>"};
  _cmds["city"] = {&sys::Console::city, "city details <city> | list | stock <city>"};
  _cmds["exit"] = {NULL, "exit"};
  _cmds["help"] = {&sys::Console::help, "help"};
  _cmds["sell"] = {&sys::Console::sell, "sell <resource> <quantity>"};
  _cmds["ship"] = {&sys::Console::ship, "ship add <type> <x> <y> | details | list | move (<city> | <x> <y>) | select <id>"};
  _cmds["status"] = {&sys::Console::status, "status"};
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
    else if (_cmds[cmd].ft != NULL) {
      try {
	(this->*_cmds[cmd].ft)(ss);
      } catch (Exception::Usage const&) {
	std::cerr << "Usage: " << _cmds[cmd].usage << std::endl;
      } catch (std::exception const& e) {
	std::cerr << e.what() << std::endl;
      }
    }
    else
      std::cerr << "Unknown command " << cmd << std::endl;
  }
  _w->stop();
}

void sys::Console::update(Ecs::World& w) {
  if (!_player)
    _player = *::players.begin();
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
