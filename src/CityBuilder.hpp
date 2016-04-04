#pragma once

#include <string>
#include <map>
#include "World.hpp"

extern std::map<std::string const, unsigned int> cityNames;

class CityBuilder {
private:
  static std::string const getName();

public:
  static Ecs::Entity *random();
  static unsigned int add(Ecs::World &w);
};
