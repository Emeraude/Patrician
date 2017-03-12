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

Ecs::Entity *ShipBuilder::core(Ecs::World& w, uint32_t owner, uint16_t x, uint16_t y) {
  Ecs::Entity *e = new Ecs::Entity();

  e->add<comp::Position>(x, y);
  e->add<comp::Type>(Type::SHIP);
  e->add<comp::Name>(randomName());
  e->add<comp::Stock>();
  e->add<comp::Owner>(owner);
  e->add<comp::Id>(w.add(e));
  return e;
}

Ecs::Entity *ShipBuilder::addSnaikka(Ecs::World& w, uint32_t owner, uint16_t x, uint16_t y) {
  Ecs::Entity *e = core(w, owner, x, y);

  e->add<comp::Speed>(6);
  e->add<comp::Hull>(80);
  return e;
}

Ecs::Entity *ShipBuilder::addCrayer(Ecs::World& w, uint32_t owner, uint16_t x, uint16_t y) {
  Ecs::Entity *e = core(w, owner, x, y);

  e->add<comp::Speed>(5);
  e->add<comp::Hull>(100);
  return e;
}
