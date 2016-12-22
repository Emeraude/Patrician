#include <exception>
#include "CityBuilder.hpp"
#include "BuildingBuilder.hpp"
#include "Components.hpp"

#define SIZE(x) (sizeof(x) / sizeof(*x))

std::map<std::string const, unsigned int> cityNames;

std::string const CityBuilder::getName() {
  static const std::string names[] = {
    "Lüebeck", "Gdansk", "Stockholm", "Edimburg"
  };
  static unsigned int i = 0;

  if (i >= SIZE(names))
    throw std::range_error("All city names already used");
  return names[i++];
}

unsigned int CityBuilder::addRandom(Ecs::World& w) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Position>(rand() % 1000, rand() % 1000);
  e->addComponent<comp::Type>(Type::CITY);
  e->addComponent<comp::Name>(getName());
  e->addComponent<comp::Inhabitants>(rand() % 100 + 1950,
				     rand() % 70 + 860,
				     rand() % 50 + 470,
				     rand() % 10 + 85);
  e->addComponent<comp::Buildings>();

  unsigned int id = w.addEntity(e);
  unsigned int idOffice = BuildingBuilder::addOffice(w, id, id);
  e->getComponent<comp::Buildings>()->office = idOffice;
  comp::Stock *s = w.getEntity(idOffice)->getComponent<comp::Stock>();
  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    s->at(static_cast<Resource>(i)).quantity = 5;
  }

  e->addComponent<comp::Id>(id);
  ::cityNames[e->getComponent<comp::Name>()->value] = id;
  return id;
}
