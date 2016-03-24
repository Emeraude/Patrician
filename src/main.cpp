#include <random>
#include "World.hpp"
#include "systems/console.cpp"

int main(void) {
  Ecs::World w;
  std::random_device rd;
  std::srand(rd());

  w.addSystem<System::Console>();
  w.run();
  return 0;
}
