#pragma once

#include <cstdint>
#include <map>
#include <string>

enum Resource {
  BRICK = 0, IRON_GOODS, TIMBER,
  FIRST = BRICK, LAST = TIMBER,
  LENGTH = LAST + 1
};

static const struct {
  char const* name;
  uint8_t size;
  uint16_t cost;
  struct {
    uint16_t base;
    uint16_t poor;
    uint16_t wealthy;
    uint16_t rich;
  } consumption;
} infosResource[] = {
  {"brick", 10, 150, {7, 1, 0, 0}},
  {"iron goods", 1, 300, {20, 0, 0, 0}},
  {"timber", 10, 100, {5, 1, 0, 0}}
};

static const std::map<std::string, Resource> resourceNames {
  {"brick", Resource::BRICK},
  {"iron goods", Resource::IRON_GOODS},
  {"timber", Resource::TIMBER}
};
