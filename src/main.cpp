#include <exception>
#include <iostream>
#include <random>
#include "World.hpp"
#include "systems/Console.cpp"
#include "systems/Move.cpp"

int main(void) {
  Ecs::World w;
  std::random_device rd;
  std::srand(rd());

  w.addSystem<System::Console>();
  w.addSystem<System::Move>();
  try {
    w.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
