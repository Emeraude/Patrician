#include "Duration.hpp"

Duration::Duration(uint32_t years, uint32_t months, uint32_t days, uint32_t hours, uint32_t minutes)
  : _years(years), _months(months), _days(days), _hours(hours), _minutes(minutes) {
  this->simplify();
}

void Duration::simplify() {
  if (_minutes >= 60) {
    _hours += _minutes / 60;
    _minutes %= 60;
  }
  if (_hours >= 24) {
    _days += _hours / 24;
    _hours %= 24;
  }
  if (_days > 30) {
    _months += _days / 30;
    _days %= 30;
  }
  if (_months >= 12) {
    _years += _months / 12;
    _months %= 12;
  }
}

Duration Duration::operator+(Duration const& rhs) {
  return Duration(this->_years + rhs._years,
		  this->_months + rhs._months,
		  this->_days + rhs._days,
		  this->_hours + rhs._hours,
		  this->_minutes + rhs._minutes);
}

Duration& Duration::operator+=(Duration const& rhs) {
  _minutes += rhs._minutes;
  _hours += rhs._hours;
  _days += rhs._days;
  _months += rhs._months;
  _years += rhs._years;
  this->simplify();
  return *this;
}

Duration& Duration::operator++(int) {
  _minutes += 10;
  this->simplify();
  return *this;
}

Duration& Duration::operator++() {
  _minutes += 10;
  this->simplify();
  return *this;
}
