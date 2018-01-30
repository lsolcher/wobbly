#pragma once
#include "Arduino.h"
#include "Entity.h"


class SpawnPoint: public Entity {
  private:
    short _spawnSpeed = 0;
    short _spawnTick = 0;
    CRGB::HTMLColorCode _spawnColor;

  public:
    void setSpawnSpeed(short speed);
    bool spawns();
    short getSpawnTick();
    void setSpawnColor(CRGB::HTMLColorCode color);
    CRGB::HTMLColorCode getSpawnColor();
    SpawnPoint(int position, CRGB::HTMLColorCode color, char* type);


};

SpawnPoint::SpawnPoint(int position, CRGB::HTMLColorCode color, char* type) : Entity(position, color, type) {
  _spawnSpeed = 0;
  _spawnTick = 10;
}
void SpawnPoint::setSpawnSpeed(short speed) {
  _spawnSpeed = speed;
  _spawnTick = speed / 2;
}


bool SpawnPoint::spawns() {
  if(--_spawnTick <= 0) {
    _spawnTick = _spawnSpeed;
    return true;
  } else {
    return false;
  }
}
short SpawnPoint::getSpawnTick() {
  return _spawnTick;
}

void SpawnPoint::setSpawnColor(CRGB::HTMLColorCode color){
  _spawnColor = color;
}

CRGB::HTMLColorCode SpawnPoint::getSpawnColor() {
  return _spawnColor;
}
