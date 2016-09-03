#include "Components.hpp"
#include "BuildingBuilder.hpp"

unsigned int BuildingBuilder::addOffice(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Type>(Type::BUILDING);
  e->addComponent<comp::City>(city);
  e->addComponent<comp::Owner>(owner);
  e->addComponent<comp::Name>("Office");
  if (city == owner)
    e->addComponent<comp::Size>();
  else
    e->addComponent<comp::Size>(500);
  e->addComponent<comp::Stock>();

  unsigned int id = w.addEntity(e);
  if (city == owner)
    w.getEntities()[city]->getComponent<comp::Buildings>()->office = id;
  return id;
}

Ecs::Entity* BuildingBuilder::Production::base(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Type>(Type::BUILDING);
  e->addComponent<comp::City>(city);
  e->addComponent<comp::Owner>(owner);
  return e;
}

unsigned int BuildingBuilder::Production::addSawmill(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = base(w, city, owner);

  e->addComponent<comp::Name>("Sawmill");
  e->addComponent<comp::Production>();
  e->getComponent<comp::Production>()->at(Resource::TIMBER) = 5;
  return w.addEntity(e);
}
