#include <exception>
#include <iostream>
#include <random>
#include "World.hpp"
#include "systems/Console.hpp"
#include "systems/Sdl.hpp"
#include "systems/Time.hpp"
#include "Arguments.hpp"
#include "CityBuilder.hpp"
#include "PlayerBuilder.hpp"

int main(int argc, char **argv) {
  try {
    Arguments args(argc, argv);
    Ecs::World w;
    std::random_device rd;
    std::srand(rd());

    w.add<sys::Console>();
    if (args.hasGui())
      w.add<sys::Sdl>(640, 480);
    w.add<sys::Time>();
    CityBuilder::addRandom(w);
    PlayerBuilder::addPlayer(w, cityNames.begin()->second);
    CityBuilder::addRandom(w);
    CityBuilder::addRandom(w);
    w.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
