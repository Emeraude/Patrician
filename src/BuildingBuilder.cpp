#include "Components.hpp"
#include "BuildingBuilder.hpp"
#include "Exceptions.hpp"

Ecs::Entity* BuildingBuilder::base(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = new Ecs::Entity();

  e->add<comp::Type>(Type::BUILDING);
  e->add<comp::City>(city);
  e->add<comp::Owner>(owner);
  return e;
}

unsigned int BuildingBuilder::addOffice(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = base(w, city, owner);

  e->add<comp::Name>("Office");
  if (city == owner)
    e->add<comp::Size>();
  else
    e->add<comp::Size>(500);
  e->add<comp::Stock>();

  unsigned int id = w.add(e);
  if (city == owner)
    w.getEntities()[city]->get<comp::Buildings>()->office = id;
  e->add<comp::Id>(id);
  return id;
}

Ecs::Entity* BuildingBuilder::Production::base(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::base(w, city, owner);
  int id = -1;

  for (auto* it : w.getEntities()) {
    if (it->get<comp::Type>()->type == Type::BUILDING
	&& it->has<comp::Stock>()
	&& it->get<comp::City>()->id == city
	&& it->get<comp::Owner>()->id == owner) {
      id = it->get<comp::Id>()->value;
      break;
    }
  }
  if (id == -1)
    throw Exception::Building("There is no office in this town");
  e->add<comp::Office>(id);
  return e;
}

unsigned int BuildingBuilder::Production::addSawmill(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::Production::base(w, city, owner);

  e->add<comp::Name>("Sawmill");
  e->add<comp::Production>();
  e->get<comp::Production>()->at(Resource::TIMBER) = 5;
  return w.add(e);
}

unsigned int BuildingBuilder::Production::addBrickworks(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::Production::base(w, city, owner);

  e->add<comp::Name>("Brickworks");
  e->add<comp::Production>();
  e->get<comp::Production>()->at(Resource::TIMBER) = -1;
  e->get<comp::Production>()->at(Resource::BRICK) = 5;
  return w.add(e);
}
