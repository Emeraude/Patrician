#include "PlayerBuilder.hpp"
#include "BuildingBuilder.hpp"
#include "Components.hpp"

std::vector<unsigned int> players;

Ecs::Entity *PlayerBuilder::addPlayer(Ecs::World& w, uint32_t city) {
  Ecs::Entity *e = new Ecs::Entity();

  e->add<comp::Type>(Type::PLAYER);
  e->add<comp::City>(city);
  e->add<comp::Money>(10000);

  unsigned int id = w.add(e);
  e->add<comp::Id>(id);
  ::players.push_back(id);

  BuildingBuilder::addOffice(w, city, id);
  return e;
}
