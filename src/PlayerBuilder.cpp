#include "PlayerBuilder.hpp"
#include "Components.hpp"

std::vector<unsigned int> players;

Ecs::Entity *PlayerBuilder::addPlayer(Ecs::World& w, uint32_t city) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Type>(Type::PLAYER);
  e->addComponent<comp::City>(city);
  e->addComponent<comp::Money>(10000);

  unsigned int id = w.addEntity(e);
  e->addComponent<comp::Id>(id);
  ::players.push_back(id);
  return e;
}
