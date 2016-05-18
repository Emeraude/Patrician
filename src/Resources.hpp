#pragma once

#include <cstdint>

enum Resource {
  TIMBER = 0, BRICK,
  FIRST = TIMBER, LAST = BRICK,
  LENGTH = LAST + 1
};

static const struct {
  char const* name;
  uint8_t size;
  struct {
    uint16_t base;
    uint16_t poor;
    uint16_t wealthy;
    uint16_t rich;
  } consumption;
} infosResource[] = {
  {"timber", 10, {5, 1, 0, 0}},
  {"brick", 10, {7, 1, 0, 0}}
};
