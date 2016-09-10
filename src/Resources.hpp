#pragma once

#include <cstdint>
#include <map>
#include <string>

enum Resource {
  TIMBER = 0, BRICK,
  FIRST = TIMBER, LAST = BRICK,
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
  {"timber", 10, 100, {5, 1, 0, 0}},
  {"brick", 10, 150, {7, 1, 0, 0}}
};

static const std::map<std::string, Resource> resourceNames {
  {"timber", Resource::TIMBER},
  {"brick", Resource::BRICK}
};
