#include <exception>
#include <iostream>
#include <random>
#include "World.hpp"
#include "systems/Console.hpp"
#include "systems/Move.hpp"
#include "systems/Sdl.hpp"
#include "systems/Time.hpp"
#include "CityBuilder.hpp"
#include "PlayerBuilder.hpp"

int main(void) {
  try {
    Ecs::World w;
    std::random_device rd;
    std::srand(rd());

    w.add<sys::Console>();
    w.add<sys::Move>();
    w.add<sys::Sdl>();
    w.add<sys::Time>();
    CityBuilder::addRandom(w);
    PlayerBuilder::addPlayer(w, cityNames.begin()->second);
    CityBuilder::addRandom(w);
    CityBuilder::addRandom(w);
    w.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
