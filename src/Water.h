#pragma once
#include "Arduino.h"
#include "Entity.h"


class Water: public Entity {
  private:
    uint8_t _size; //gets bigger depending on size
    bool _direction; //0 == down, 1 == up
  public:
    Water(int position, CRGB::HTMLColorCode color, char* type);
    void setSize(uint8_t size);
    void setDirection(bool dir);
    uint8_t getSize();
    bool getDirection();
};

Water::Water(int position, CRGB::HTMLColorCode color, char* type) : Entity(position, color, type) {
  _size = 0;
}

void Water::setSize(uint8_t size) {
  _size = size;
}

void Water::setDirection(bool dir) {
  _direction = dir;
}

uint8_t Water::getSize() {
  return _size;
}

bool Water::getDirection() {
  return _direction;
}
