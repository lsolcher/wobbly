#pragma once
#include "Arduino.h"
#include "FastLED.h"

class Entity {
protected:
  int _position;
  CRGB::HTMLColorCode _color;
  char* _type;
public:
  Entity(int position, CRGB::HTMLColorCode color, char* type);
  Entity();
  void create(int position, CRGB::HTMLColorCode color);
  int getPosition();
  void setPosition(int position);
  void setColor(CRGB::HTMLColorCode color);
  virtual void setActive(int tick) {};
  char* getType();
  CRGB::HTMLColorCode getColor();
};
Entity::Entity() {

}

Entity::Entity(int position, CRGB::HTMLColorCode color, char* type) {
  _position = position;
  _color = color;
  _type = type;
}

char* Entity::getType() {
  return _type;
}

void Entity::create(int position, CRGB::HTMLColorCode color)  {
  _position = position;
  _color = color;
}

void Entity::setPosition(int position){
  _position = position;
}


int Entity::getPosition() {
  return _position;
}


void Entity::setColor(CRGB::HTMLColorCode color){
  _color = color;
}



CRGB::HTMLColorCode Entity::getColor() {
  return _color;
}
