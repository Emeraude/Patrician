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

  e->add<comp::Position>(rand() % 1000, rand() % 1000);
  e->add<comp::Type>(Type::CITY);
  e->add<comp::Name>(getName());
  e->add<comp::Inhabitants>(rand() % 100 + 1950,
				     rand() % 70 + 860,
				     rand() % 50 + 470,
				     rand() % 10 + 85);
  e->add<comp::Buildings>();

  unsigned int id = w.add(e);
  unsigned int idOffice = BuildingBuilder::addOffice(w, id, id);
  e->get<comp::Buildings>()->office = idOffice;
  comp::Stock *s = w.get(idOffice)->get<comp::Stock>();
  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    s->at(static_cast<Resource>(i)).quantity = 5;
  }

  e->add<comp::Id>(id);
  ::cityNames[e->get<comp::Name>()->value] = id;
  return id;
}
