#include <iostream>
#include <string>
#include "console.hpp"

System::Console::Console() : _thread(new std::thread(&System::Console::readCin, this)), _w(NULL) {}

System::Console::~Console() {
  delete _thread;
}

void System::Console::readCin() {
  std::string s;

  while(true) {
    std::cin >> s;
    if (s == "exit") {
      _w->stop();
      return;
    }
    std::cout << s << std::endl;
  }
}

void System::Console::update(Ecs::World& w) {
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
