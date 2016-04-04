#include <cmath>
#include "Time.hpp"
#include "Components.hpp"
#include "CityBuilder.hpp"

System::Time::Time() : _day(0) {}

// Consumption is too heavy, have to find a way to consume less than 1 good per day
void System::Time::consumption(Ecs::Entity *city) {
  Component::Stock& s = *city->getComponent<Component::Stock>();
  Component::Inhabitants& inhabitants = *city->getComponent<Component::Inhabitants>();

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

void System::Time::update(Ecs::World& w) {
  static unsigned int i = 0;

  // 1 day ~= 10 seconds
  if (++i < 1000)
    return;
  ++_day;
  i = 0;
  for (auto& it : cityNames)
    this->consumption(w.getEntities()[it.second]);
}

uint32_t const& System::Time::getDay() const {
  return _day;
}
