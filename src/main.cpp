#include <exception>
#include <iostream>
#include <random>
#include "World.hpp"
#include "systems/Console.cpp"
#include "systems/Move.cpp"
#include "systems/Time.cpp"
#include "CityBuilder.hpp"

int main(void) {
  try {
    Ecs::World w;
    std::random_device rd;
    std::srand(rd());

    w.addSystem<sys::Console>();
    w.addSystem<sys::Move>();
    w.addSystem<sys::Time>();
    CityBuilder::addRandom(w);
    CityBuilder::addRandom(w);
    CityBuilder::addRandom(w);
    w.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
