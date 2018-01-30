#pragma once
#include "Arduino.h"
#include "Entity.h"


class Trap: public Entity {
  private:
    short _size;
    CRGB::HTMLColorCode _inactiveColor;
    CRGB::HTMLColorCode _activeColor;
    bool _active;
  public:
    Trap(int position, CRGB::HTMLColorCode color, char* type);
    int move(int playerPosition, int tick);
    void setChangesDirection(bool changesDirection);
    void setSize(short size);
    void setInactiveColor(CRGB::HTMLColorCode color);
    void setActiveColor(CRGB::HTMLColorCode color);
    CRGB::HTMLColorCode getInactiveColor();
    void setActive(int tick);
    bool isActive();
};

Trap::Trap(int position, CRGB::HTMLColorCode color, char* type) : Entity(position, color, type) {
  _active = false;
  _inactiveColor = CRGB::HTMLColorCode::White;
  _activeColor = CRGB::HTMLColorCode::Black;
}

void Trap::setActive(int tick) {
  (tick % 200 > 100) ? _active = true : _active = false;
  (_active == true) ? setColor(_activeColor) : setColor(_inactiveColor);
}

bool Trap::isActive() {
  return _active;
}

void Trap::setSize(short size) {
  _size = size;
}

void Trap::setInactiveColor(CRGB::HTMLColorCode color){
  _inactiveColor = color;
}

void Trap::setActiveColor(CRGB::HTMLColorCode color){
  _activeColor = color;
}
