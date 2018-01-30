#pragma once
#include "Arduino.h"
#include "Entity.h"


class Enemy: public Entity {
  private:
    //int position;
    bool _changesDirection = false;
    int _speed;
    bool _moves;
  public:
    int move(int playerPosition, int tick);
    void setChangesDirection(bool changesDirection);
    Enemy(int position, CRGB::HTMLColorCode color, char* type);
    void setMoves(bool moves);

};

Enemy::Enemy(int position, CRGB::HTMLColorCode color, char* type) : Entity(position, color, type) {
  _changesDirection = false;
  _moves = true;
}

int Enemy::move(int playerPosition, int tick) {
  if(_moves) {
    int newPos;
    if(_changesDirection && tick % 100 > 60)
      (playerPosition < _position) ? newPos = _position++ : newPos = _position--;
    else
      (playerPosition < _position) ? newPos = _position-- : newPos = _position++;
    return newPos;
  } else {
    return _position;
  }
}

void Enemy::setChangesDirection(bool changesDirection) {
  _changesDirection = changesDirection;
}

void Enemy::setMoves(bool moves) {
  _moves = moves;
}



/*void Enemy::setPosition(int thePosition){
  position = thePosition;
}



int Enemy::getPosition() {
  return position;
}*/
