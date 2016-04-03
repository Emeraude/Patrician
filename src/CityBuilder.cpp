#include <exception>
#include "CityBuilder.hpp"
#include "Components.hpp"

#define SIZE(x) (sizeof(x) / sizeof(*x))

std::string const CityBuilder::getName() {
  static const std::string names[] = {
    "Lüebeck", "Gdansk", "Stockholm", "Edimburg"
  };
  static unsigned int i = 0;

  if (i >= SIZE(names))
    throw std::range_error("All city names already used");
  return names[i++];
}

Ecs::Entity *CityBuilder::random() {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<Component::Position>(rand() % 1000, rand() % 1000);
  e->addComponent<Component::Type>(Type::CITY);
  e->addComponent<Component::Name>(getName());
  e->addComponent<Component::Inhabitants>(rand() % 100 + 1950,
					  rand() % 70 + 860,
					  rand() % 50 + 470,
					  rand() % 10 + 85);
  e->addComponent<Component::Stock>();
  e->getComponent<Component::Stock>()->at(Resource::TIMBER) = 5;
  e->getComponent<Component::Stock>()->at(Resource::BRICK) = 5;
  return e;
}
