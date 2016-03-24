#include "ShipBuilder.hpp"
#include "Components.hpp"

std::string const ShipBuilder::randomName() {
  return "toto";
}

Ecs::Entity *ShipBuilder::Crayer(int16_t x, int16_t y) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<Component::Position>(x, y);
  e->addComponent<Component::Type>(Type::SHIP);
  e->addComponent<Component::Name>(randomName());
  return e;
}
