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
  sys::Time& sys = *_w->getSystem<sys::Time>();

  std::cout << "Player #" << _player << std::endl
	    << "Day " << sys.getDay() << std::endl
	    << "Speed: 1 day ~= " << (sys.getSpeed() / 100.0) << " seconds" << std::endl;
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
	  throw Exception::Usage("");
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
      this->checkSelectedShip();
      Ecs::Entity& e = *_w->getEntities().at(_selectedShip);
      e.addComponent<comp::Move>(x, y);
      e.getComponent<comp::Speed>()->current = e.getComponent<comp::Speed>()->max;
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
	if (e.hasComponent<comp::Type>()
	    && e.getComponent<comp::Type>()->type != Type::SHIP)
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
    std::cout << "Ship \"" << e.getComponent<comp::Name>()->value << "\" #" << _selectedShip
	      << " in " << e.getComponent<comp::Position>()->x
	      << "," << e.getComponent<comp::Position>()->y;
    if (e.hasComponent<comp::City>())
      std::cout << " (" << _w->getEntity(e.getComponent<comp::City>()->id)->getComponent<comp::Name>()->value << ")";
    std::cout << std::endl;
    comp::Stock *s = e.getComponent<comp::Stock>();
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
      Ecs::Entity *city = _w->getEntity(it.second);
      std::cout << "\"" << city->getComponent<comp::Name>()->value
		<< "\" (" << city->getComponent<comp::Inhabitants>()->total()
		<< ") in " << city->getComponent<comp::Position>()->x
		<< "," << city->getComponent<comp::Position>()->y << std::endl;
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
      throw Exception::Usage("");
    else {
      try {
	Ecs::Entity *city = _w->getEntity(::cityNames.at(cityName));
	Ecs::Entity *e = _w->getEntity(city->getComponent<comp::Buildings>()->office);
	comp::Stock *s = e->getComponent<comp::Stock>();
	for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i)
	  std::cout << infosResource[i].name << " " << s->at(static_cast<Resource>(i)).quantity << std::endl;
      } catch (std::out_of_range&) {
	std::cerr << "No city \"" << cityName << "\" found" << std::endl;
      }
    }
  }
  else
    std::cerr << "Usage: city stock <city>" << std::endl;
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
      std::cerr << "Usage: building create <city> <type>" << std::endl;
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

void sys::Console::buy(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string resourceStr;
  Resource resource;
  uint32_t quantity;
  if (!(ss >> resourceStr >> quantity))
    throw Exception::Usage("");
  try {
    resource = resourceNames.at(resourceStr);
  } catch (std::out_of_range&) {
    std::cerr << "Resource \"" << resourceStr << "\" does not exist" << std::endl;
    return;
  }
  this->checkSelectedShip();
  Ecs::Entity *ship = _w->getEntity(_selectedShip);
  if (!ship->hasComponent<comp::City>()) {
    std::cerr << "Ship #" << _selectedShip << " is not in a city" << std::endl;
    return;
  }
  Ecs::Entity *city = _w->getEntity(ship->getComponent<comp::City>()->id);
  Ecs::Entity *office = _w->getEntity(city->getComponent<comp::Buildings>()->office);
  Ecs::Entity *player = _w->getEntity(_player);
  comp::Stock *cityStock = office->getComponent<comp::Stock>();
  unsigned int price = infosResource[resource].cost * quantity;
  if (cityStock->at(resource).quantity < quantity) {
    std::cerr << "City only have " << cityStock->at(resource).quantity
	      << " " << resourceStr << std::endl;
  }
  else if (player->getComponent<comp::Money>()->value < price) {
    std::cerr << "You don't have enough money. It costs " << price
	      << " and you have " << player->getComponent<comp::Money>()->value << std::endl;
  }
  else {
    player->getComponent<comp::Money>()->value -= price;
    cityStock->at(resource).quantity -= quantity;
    ship->getComponent<comp::Stock>()->at(resource).quantity += quantity;
  }
}

void sys::Console::checkSelectedShip() {
  try {
    Ecs::Entity *ship = _w->getEntity(_selectedShip);
    if (!ship->hasComponent<comp::Type>()
	|| ship->getComponent<comp::Type>()->type != Type::SHIP)
      throw std::out_of_range("");
  } catch (std::out_of_range&) {
    throw ::Exception::Ship("No ship selected");
  }
}

void sys::Console::sell(std::stringstream& ss) {
  if (ss.eof())
    throw Exception::Usage("");
  std::string resourceStr;
  Resource resource;
  uint32_t quantity;
  if (!(ss >> resourceStr >> quantity))
    throw Exception::Usage("");
  try {
    resource = resourceNames.at(resourceStr);
  } catch (std::out_of_range&) {
    std::cerr << "Resource \"" << resourceStr << "\" does not exist" << std::endl;
    return;
  }
  this->checkSelectedShip();
  Ecs::Entity *ship = _w->getEntity(_selectedShip);
  if (!ship->hasComponent<comp::City>()) {
    std::cerr << "Ship #" << _selectedShip << " is not in a city" << std::endl;
    return;
  }
  Ecs::Entity *city = _w->getEntity(ship->getComponent<comp::City>()->id);
  Ecs::Entity *office = _w->getEntity(city->getComponent<comp::Buildings>()->office);
  Ecs::Entity *player = _w->getEntity(_player);
  comp::Stock *cityStock = office->getComponent<comp::Stock>();
  unsigned int price = infosResource[resource].cost * quantity;
  if (ship->getComponent<comp::Stock>()->at(resource).quantity < quantity) {
    std::cerr << "Your ship only have " << ship->getComponent<comp::Stock>()->at(resource).quantity
	      << " " << resourceStr << std::endl;
  }
  else {
    player->getComponent<comp::Money>()->value += price;
    cityStock->at(resource).quantity += quantity;
    ship->getComponent<comp::Stock>()->at(resource).quantity -= quantity;
  }
}

void sys::Console::readCin() {
  std::string in;

  _cmds["building"] = {&sys::Console::building, "building create <city> <type> | list <city>"};
  _cmds["buy"] = {&sys::Console::buy, "buy <resource> <quantity>"};
  _cmds["city"] = {&sys::Console::city, "city details <city> | list | stock <city>"};
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
