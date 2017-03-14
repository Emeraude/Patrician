#pragma once

#include <cstdint>

class Duration {
private:
  uint32_t _years;
  uint32_t _months;
  uint32_t _days;
  uint32_t _hours;
  uint32_t _minutes;

  void simplify();
public:
  Duration(uint32_t years, uint32_t months = 0, uint32_t days = 0, uint32_t hours = 0, uint32_t minutes = 0);
  Duration operator+(Duration const& rhs);
  Duration& operator+=(Duration const& rhs);
  Duration& operator++(int);
  Duration& operator++();
};
