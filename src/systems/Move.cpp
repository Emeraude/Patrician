#include <cmath>
#include "Move.hpp"
#include "Components.hpp"
#include "CityBuilder.hpp"

void sys::Move::checkCities(Ecs::World &w, Ecs::Entity *e) {
  comp::Position *posE = e->getComponent<comp::Position>();
  for (auto& it : ::cityNames) {
    Ecs::Entity *city = w.getEntity(it.second);
    comp::Position *posCity = city->getComponent<comp::Position>();
    if (posE->x == posCity->x
	&& posE->y == posCity->y)
      e->addComponent<comp::City>(it.second);
  }
}

// Too fast now, find a way to slower it
void sys::Move::update(Ecs::World &w) {
  for (auto *it: w.getEntities()) {
    if (it->hasComponent<comp::Move>()) {
      comp::Move *move = it->getComponent<comp::Move>();
      comp::Position *pos = it->getComponent<comp::Position>();

      if (it->hasComponent<comp::City>())
	it->removeComponent<comp::City>();

      int16_t  diff_x, diff_y;
      diff_x = move->x - pos->x;
      diff_y = move->y - pos->y;

      int16_t dist = sqrt(diff_x * 2 + diff_y * 2);
      int16_t turns = dist / it->getComponent<comp::Speed>()->current;
      if (turns == 0) {
	it->getComponent<comp::Speed>()->current = 0;
	pos->x = move->x;
	pos->y = move->y;
	it->removeComponent<comp::Move>();
	this->checkCities(w, it);
      }
      else {
	pos->x += diff_x / turns;
	pos->y += diff_y / turns;
      }
    }
  }
}
