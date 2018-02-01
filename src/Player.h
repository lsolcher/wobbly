#pragma once
#include "Arduino.h"
#include "Entity.h"
#define LED_NUM             150


#define MOVEMENTSPEED 10; //the higher, the slower

class Player: public Entity {
  private:
    short _lives = 3;
  public:
    short getLives();
    void setLives(short lives);
    void move(int input);
};


void Player::move(int input) {
  int moveAmt = input/MOVEMENTSPEED;
  if(moveAmt > 3)
    moveAmt = 3;
  _position += moveAmt;
  (_position > 999) ? _position = 999 : _position = _position;
  (_position < 0) ? _position = 0 : _position = _position;

}

void Player::setLives(short lives){
  _lives = lives;
}

short Player::getLives() {
  return _lives;
}
