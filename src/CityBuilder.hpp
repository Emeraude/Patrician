#pragma once

#include <string>
#include "World.hpp"

class CityBuilder {
private:
  static std::string const getName();

public:
  static Ecs::Entity *random();
};
