#include <cmath>
#include "Move.hpp"
#include "Components.hpp"
#include "CityBuilder.hpp"

void sys::Move::checkCities(Ecs::World &w, Ecs::Entity *e) {
  comp::Position *posE = e->get<comp::Position>();
  for (auto& it : ::cityNames) {
    Ecs::Entity *city = w.get(it.second);
    comp::Position *posCity = city->get<comp::Position>();
    if (posE->x == posCity->x
	&& posE->y == posCity->y)
      e->add<comp::City>(it.second);
  }
}

// Too fast now, find a way to slower it
void sys::Move::update(Ecs::World &w) {
  for (auto *it: w.getEntities()) {
    if (it->has<comp::Move>()) {
      comp::Move *move = it->get<comp::Move>();
      comp::Position *pos = it->get<comp::Position>();

      if (it->has<comp::City>())
	it->remove<comp::City>();

      int16_t  diff_x, diff_y;
      diff_x = move->x - pos->x;
      diff_y = move->y - pos->y;

      int16_t dist = sqrt(diff_x * 2 + diff_y * 2);
      int16_t turns = dist / it->get<comp::Speed>()->current;
      if (turns == 0) {
	it->get<comp::Speed>()->current = 0;
	pos->x = move->x;
	pos->y = move->y;
	it->remove<comp::Move>();
	this->checkCities(w, it);
      }
      else {
	pos->x += diff_x / turns;
	pos->y += diff_y / turns;
      }
    }
  }
}
