#include "Components.hpp"
#include "BuildingBuilder.hpp"
#include "Exceptions.hpp"

Ecs::Entity* BuildingBuilder::base(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Type>(Type::BUILDING);
  e->addComponent<comp::City>(city);
  e->addComponent<comp::Owner>(owner);
  return e;
}

unsigned int BuildingBuilder::addOffice(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = base(w, city, owner);

  e->addComponent<comp::Name>("Office");
  if (city == owner)
    e->addComponent<comp::Size>();
  else
    e->addComponent<comp::Size>(500);
  e->addComponent<comp::Stock>();

  unsigned int id = w.addEntity(e);
  if (city == owner)
    w.getEntities()[city]->getComponent<comp::Buildings>()->office = id;
  e->addComponent<comp::Id>(id);
  return id;
}

Ecs::Entity* BuildingBuilder::Production::base(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::base(w, city, owner);
  int id = -1;

  for (auto* it : w.getEntities()) {
    if (it->getComponent<comp::Type>()->type == Type::BUILDING
	&& it->hasComponent<comp::Stock>()
	&& it->getComponent<comp::City>()->id == city
	&& it->getComponent<comp::Owner>()->id == owner)
      id = it->getComponent<comp::Id>()->value;
  }
  if (id == -1)
    throw Exception::Building("There is no office in this town");
  e->addComponent<comp::Office>(id);
  return e;
}

unsigned int BuildingBuilder::Production::addSawmill(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::Production::base(w, city, owner);

  e->addComponent<comp::Name>("Sawmill");
  e->addComponent<comp::Production>();
  e->getComponent<comp::Production>()->at(Resource::TIMBER) = 5;
  return w.addEntity(e);
}

unsigned int BuildingBuilder::Production::addBrickworks(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = BuildingBuilder::Production::base(w, city, owner);

  e->addComponent<comp::Name>("Brickworks");
  e->addComponent<comp::Production>();
  e->getComponent<comp::Production>()->at(Resource::TIMBER) = -1;
  e->getComponent<comp::Production>()->at(Resource::BRICK) = 5;
  return w.addEntity(e);
}
