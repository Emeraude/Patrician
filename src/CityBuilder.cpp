#include "CityBuilder.hpp"
#include "Components.hpp"

#define SIZE(x) (sizeof(x) / sizeof(*x))

std::string const CityBuilder::getName() {
  static const std::string names[] = {
    "Lüebeck", "Gdansk", "Stockholm", "Edimburg"
  };
  static unsigned int i = 0;

  if (i >= SIZE(names))
    return "unamed";
  return names[i++];
}

Ecs::Entity *CityBuilder::random() {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<Component::Position>(rand() % 1000, rand() % 1000);
  e->addComponent<Component::Type>(Type::CITY);
  e->addComponent<Component::Name>(getName());
  return e;
}
