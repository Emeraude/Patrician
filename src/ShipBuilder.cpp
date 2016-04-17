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

Ecs::Entity *ShipBuilder::core(uint16_t x, uint16_t y) {
  Ecs::Entity *e = new Ecs::Entity();

  e->addComponent<comp::Position>(x, y);
  e->addComponent<comp::Type>(Type::SHIP);
  e->addComponent<comp::Name>(randomName());
  e->addComponent<comp::Stock>();
  return e;
}

Ecs::Entity *ShipBuilder::Snaikka(uint16_t x, uint16_t y) {
  Ecs::Entity *e = core(x, y);

  e->addComponent<comp::Speed>(6);
  e->addComponent<comp::Hull>(80);
  return e;
}

Ecs::Entity *ShipBuilder::Crayer(uint16_t x, uint16_t y) {
  Ecs::Entity *e = core(x, y);

  e->addComponent<comp::Speed>(5);
  e->addComponent<comp::Hull>(100);
  return e;
}
