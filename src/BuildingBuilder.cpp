#include "Components.hpp"
#include "BuildingBuilder.hpp"

Ecs::Entity *BuildingBuilder::office(Ecs::World& w, uint32_t city, uint32_t owner) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Type>(Type::BUILDING);
  e->addComponent<comp::City>(city);
  e->addComponent<comp::Owner>(owner);
  if (city == owner)
    e->addComponent<comp::Size>();
  else
    e->addComponent<comp::Size>(500);
  e->addComponent<comp::Stock>();
  if (city == owner)
    w.getEntities()[city]->getComponent<comp::Buildings>()->office = w.addEntity(e);
  return e;
}
