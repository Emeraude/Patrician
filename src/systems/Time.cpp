#include "Time.hpp"

System::Time::Time() : _day(0) {}

void System::Time::update(Ecs::World &) {
  static unsigned int i = 0;

  // 1 day ~= 10 seconds
  if (++i < 1000)
    return;
  ++_day;
  i = 0;
}

uint32_t const& System::Time::getDay() const {
  return _day;
}
