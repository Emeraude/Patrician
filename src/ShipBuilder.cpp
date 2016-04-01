#include "ShipBuilder.hpp"
#include "Components.hpp"

#define SIZE(x) (sizeof(x) / sizeof(*x))

std::string const ShipBuilder::randomName() {
  static const std::string first[] = {
    "Pretty", "Huge"
  };
  static const std::string second[] = {
    "Ship", "Boat", "Vessel"
  };
  return first[std::rand() % SIZE(first)]
    + " " + second[std::rand() % SIZE(second)];
}

Ecs::Entity *ShipBuilder::Snaikka(uint16_t x, uint16_t y) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<Component::Position>(x, y);
  e->addComponent<Component::Type>(Type::SHIP);
  e->addComponent<Component::Name>(randomName());
  e->addComponent<Component::Speed>(6);
  e->addComponent<Component::Hull>(80);
  e->addComponent<Component::Stock>();
  return e;
}

Ecs::Entity *ShipBuilder::Crayer(uint16_t x, uint16_t y) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<Component::Position>(x, y);
  e->addComponent<Component::Type>(Type::SHIP);
  e->addComponent<Component::Name>(randomName());
  e->addComponent<Component::Speed>(5);
  e->addComponent<Component::Hull>(100);
  e->addComponent<Component::Stock>();
  return e;
}
