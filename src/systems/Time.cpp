#include <cmath>
#include "Time.hpp"
#include "Components.hpp"
#include "CityBuilder.hpp"

sys::Time::Time() : _date(1303), _speed(10) {}

// Consumption is too heavy, have to find a way to consume less than 1 good per day
void sys::Time::consumption(Ecs::World& w, Ecs::Entity *city) {
  Ecs::Entity *office = w.get(city->get<comp::Buildings>()->office);
  comp::Stock& s = *office->get<comp::Stock>();
  comp::Inhabitants& inhabitants = *city->get<comp::Inhabitants>();

  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    unsigned int consumed = ceil(infosResource[i].consumption.base
				  + infosResource[i].consumption.poor * inhabitants.poor / 1000.0
				  + infosResource[i].consumption.wealthy * inhabitants.wealthy / 1000.0
				  + infosResource[i].consumption.rich * inhabitants.rich / 1000.0);
    if (consumed > s.at(static_cast<Resource>(i)).quantity)
      s.at(static_cast<Resource>(i)).quantity = 0;
    else
      s.at(static_cast<Resource>(i)).quantity -= consumed;
  }
}

// Same problem as consumption one
void sys::Time::production(Ecs::World& w, Ecs::Entity *building) {
  Ecs::Entity *office = w.get(building->get<comp::Office>()->id);
  comp::Stock& stock = *office->get<comp::Stock>();
  comp::Production& prod = *building->get<comp::Production>();

  for (unsigned int i = Resource::FIRST; i <= Resource::LAST; ++i) {
    if (prod.at(static_cast<Resource>(i)) < 0
	&& stock.at(static_cast<Resource>(i)).quantity < prod.at(static_cast<Resource>(i)))
      continue;
    stock.at(static_cast<Resource>(i)).quantity += prod.at(static_cast<Resource>(i));
  }
}

void sys::Time::move(Ecs::World&w, Ecs::Entity *e) {
  comp::Move *move = e->get<comp::Move>();
  comp::Position *pos = e->get<comp::Position>();

  if (e->has<comp::City>())
    e->remove<comp::City>();

  int16_t diff_x = move->x - pos->x,
    diff_y = move->y - pos->y,
    dist = sqrt((diff_x << 2) + (diff_y << 2)),
    turns = dist / e->get<comp::Speed>()->current;
  if (turns == 0) {
    e->get<comp::Speed>()->current = 0;
    pos->x = move->x;
    pos->y = move->y;
    e->remove<comp::Move>();
    for (auto& it : ::cityNames) {
      comp::Position *posCity = w.get(it.second)->get<comp::Position>();
      if (pos->x == posCity->x
	  && pos->y == posCity->y) {
	e->add<comp::City>(it.second);
	break;
      }
    }
  }
  else {
    pos->x += diff_x / turns;
    pos->y += diff_y / turns;
  }
}

void sys::Time::update(Ecs::World& w) {
  static unsigned int i = 0;

  if (++i < _speed)
    return;
  ++_date;
  i = 0;
  for (auto* it : w.getEntities()) {
    if (it->get<comp::Type>()->type == Type::CITY)
      this->consumption(w, it);
    else if (it->get<comp::Type>()->type == Type::BUILDING
	     && it->has<comp::Production>())
      this->production(w, it);
    if (it->has<comp::Move>())
      this->move(w, it);
  }
}

Duration const& sys::Time::getDate() const {
  return _date;
}

uint32_t const& sys::Time::getSpeed() const {
  return _speed;
}
