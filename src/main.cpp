#include "World.hpp"
#include "systems/console.cpp"

int main(void) {
  Ecs::World w;

  w.addSystem<System::Console>();
  w.run();
  return 0;
}
