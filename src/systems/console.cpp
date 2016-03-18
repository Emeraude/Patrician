#include <iostream>
#include <string>
#include "console.hpp"

System::Console::Console() : _thread(new std::thread(&System::Console::readCin, this)), _w(NULL) {}

System::Console::~Console() {
  delete _thread;
}

void System::Console::readCin() {
  std::string in;

  while(true) {
    if (!std::getline(std::cin, in)) {
      _w->stop();
      return;
    }
    std::stringstream ss(in);
    std::string cmd;
    ss >> cmd;
    if (cmd == "exit") {
      _w->stop();
      return;
    }
    else if (_cmds[cmd] != NULL)
      (this->*_cmds[cmd])(ss);
    else
      std::cerr << "Unknown command " << cmd << std::endl;
    std::cout << in << std::endl;
  }
}

void System::Console::update(Ecs::World& w) {
  if (!_w) {
    _w = &w;
    _thread->detach();
  }
}
