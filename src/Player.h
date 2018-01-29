#pragma once
#include "Arduino.h"
#include "Entity.h"
#define LED_NUM             100


#define MOVEMENTSPEED 10; //the higher, the slower

class Player: public Entity {
  private:
    short _lives = 3;
    //int mapPlayer(int position);
    //int position;
  public:
    short getLives();
    void setLives(short lives);
  //  int getPosition();
    //Player();
    void move(int input);
};


void Player::move(int input) {
//  Serial.println("INPUT");
  //Serial.println(input);

  _position += input/MOVEMENTSPEED;
  (_position > 999) ? _position = 999 : _position = _position;
  (_position < 0) ? _position = 0 : _position = _position;
  //Serial.println("MOVE");
  //Serial.println(_position);

}

void Player::setLives(short lives){
  _lives = lives;
}

short Player::getLives() {
  return _lives;
}

/*int Player::mapPlayer(int position) {
  return constrain((int)map(position, 0, 1000, 0, LED_NUM-1), 0, LED_NUM-1);
}*/
