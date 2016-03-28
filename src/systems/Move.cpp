#include <cmath>
#include "Move.hpp"

// Too fast now, find a way to slower it
void System::Move::update(Ecs::World &w) {
  for (auto *it: w.getEntities()) {
    if (it->hasComponent<Component::Move>()) {
      Component::Move *move = it->getComponent<Component::Move>();
      Component::Position *pos = it->getComponent<Component::Position>();

      int16_t  diff_x, diff_y;
      diff_x = move->x - pos->x;
      diff_y = move->y - pos->y;

      int16_t dist = sqrt(diff_x * 2 + diff_y * 2);
      int16_t turns = dist / it->getComponent<Component::Speed>()->current;
      if (turns == 0) {
	it->getComponent<Component::Speed>()->current = 0;
	pos->x = move->x;
	pos->y = move->y;
	it->removeComponent<Component::Move>();
      }
      else {
	pos->x += diff_x / turns;
	pos->y += diff_y / turns;
      }
    }
  }
}
