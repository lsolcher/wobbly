#pragma once
#include "Arduino.h"
#include "Entity.h"


class Enemy: public Entity {
  private:
    //int position;
    bool _changesDirection = false;
    int _speed;
  public:
    int move(int playerPosition, int tick);
    void setChangesDirection(bool changesDirection);
    //int getPosition();
    //void setPosition(int position);
};

int Enemy::move(int playerPosition, int tick) {
  int newPos;
  if(_changesDirection && tick > 60)
    (playerPosition < _position) ? newPos = _position++ : newPos = _position--;
  else
    (playerPosition < _position) ? newPos = _position-- : newPos = _position++;
  return newPos;
}

void Enemy::setChangesDirection(bool changesDirection) {
  _changesDirection = true;
}



/*void Enemy::setPosition(int thePosition){
  position = thePosition;
}



int Enemy::getPosition() {
  return position;
}*/
