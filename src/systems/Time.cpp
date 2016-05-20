#include <cmath>
#include "Time.hpp"
#include "Components.hpp"
#include "CityBuilder.hpp"

sys::Time::Time() : _day(0), _speed(1000) {}

// Consumption is too heavy, have to find a way to consume less than 1 good per day
void sys::Time::consumption(Ecs::World& w, Ecs::Entity *city) {
  comp::Stock& s = *(w.getEntities()[city->getComponent<comp::Buildings>()->office]->getComponent<comp::Stock>());
  comp::Inhabitants& inhabitants = *city->getComponent<comp::Inhabitants>();

  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    unsigned int consumed = ceil(infosResource[i].consumption.base
				  + infosResource[i].consumption.poor * inhabitants.poor / 1000.0
				  + infosResource[i].consumption.wealthy * inhabitants.wealthy / 1000.0
				  + infosResource[i].consumption.rich * inhabitants.rich / 1000.0);
    if (consumed > s.at(static_cast<Resource>(i)))
      s.at(static_cast<Resource>(i)) = 0;
    else
      s.at(static_cast<Resource>(i)) -= consumed;
  }
}

void sys::Time::update(Ecs::World& w) {
  static unsigned int i = 0;

  if (++i < _speed)
    return;
  ++_day;
  i = 0;
  for (auto& it : ::cityNames)
    this->consumption(w, w.getEntities()[it.second]);
}

uint32_t const& sys::Time::getDay() const {
  return _day;
}

uint32_t const& sys::Time::getSpeed() const {
  return _speed;
}
