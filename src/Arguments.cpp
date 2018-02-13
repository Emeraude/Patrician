#include "Arguments.hpp"
#include <iostream>
#include <string>

Arguments::Arguments(uint32_t argc, char **argv) : _gui(true) {
  for (unsigned int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-nw"
	|| std::string(argv[i]) == "--no-window")
      _gui = false;
  }
}

bool Arguments::hasGui() const {
  return _gui;
}
