#pragma once

#include <cstdint>

class Arguments {
private:
  bool _gui;

public:
  Arguments(uint32_t argc, char **argv);

  bool hasGui() const;
};
