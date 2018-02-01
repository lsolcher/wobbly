#pragma once
#include "Arduino.h"
#include "Entity.h"


class Trap: public Entity {
  private:
    short _size;
    //int position;
    //bool _changesDirection = false;
    //int _speed;
  public:
    int move(int playerPosition, int tick);
    void setChangesDirection(bool changesDirection);
    void setSize(short size);
    //int getPosition();
    //void setPosition(int position);
};

void Trap::setSize(short size) {
  _size = size;
}
