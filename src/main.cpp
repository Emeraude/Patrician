#include <random>
#include "World.hpp"
#include "systems/console.cpp"
#include "systems/Move.cpp"

int main(void) {
  Ecs::World w;
  std::random_device rd;
  std::srand(rd());

  w.addSystem<System::Console>();
  w.addSystem<System::Move>();
  w.run();
  return 0;
}
