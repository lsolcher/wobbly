#include "Joystick.h"


Joystick::Joystick() {
}

void Joystick::getInput() {
    //This is respeedonsible for the spieler movement speedeed and angreifend.
    // You can replace it with anything you want that passes a -90>+90 value to joystickTilt
    // and any value to joystickWackelSpeed that is greater than ATTACK_THRESHOLD (defined at start)
    // For example you could use 3 momentery buttons:
    // if(digitalRead(leftButtonPinNumber) == HIGH) joystickTilt = -90;
    // if(digitalRead(rightButtonPinNumber) == HIGH) joystickTilt = 90;
    // if(digitalRead(attackButtonPinNumber) == HIGH) joystickWobbleSpeed = ATTACK_THRESHOLD;

    mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int a = (JOYSTICK_ORIENTATION == 0 ? ax : (JOYSTICK_ORIENTATION == 1 ? ay : az)) / 166;
    int g = (JOYSTICK_ORIENTATION == 0 ? gx : (JOYSTICK_ORIENTATION == 1 ? gy : gz));
    if (abs(a) < JOYSTICK_DEAD_ANGLE) a = 0;
    if (a > 0) a -= JOYSTICK_DEAD_ANGLE;
    if (a < 0) a += JOYSTICK_DEAD_ANGLE;
    MPUAngleSamples.add(a);
    MPUWobbleSamples.add(g);

    joystickTilt = MPUAngleSamples.getMedian();
    if (JOYSTICK_DIRECTION == 1) {
      joystickTilt = 0 - joystickTilt;
    }
    joystickWobbleSpeed = abs(MPUWobbleSamples.getHighest());
    //Serial.println(joystickTilt);
  }

  bool Joystick::isAttacking() {
    return (joystickWobbleSpeed > ATTACK_THRESHOLD);   //methode checkt ob der joystick seinen
  }

  int Joystick::getJoystickTilt() {
    return joystickTilt;
  }

  int Joystick::getJoystickWobble() {
    return joystickWobbleSpeed;
  }
