#include "Joystick.h"


Joystick::Joystick() {
}

void Joystick::getInput() {
    // Zum testen ohne Joystick kann man diesen auch durch drei andere Input Formen ersetzen, z.B. Buttons:
    // if(digitalRead(dataPinBackward) == HIGH) joystickTilt = -90;
    // if(digitalRead(dataPinForward) == HIGH) joystickTilt = 90;
    // if(digitalRead(dataPinAttack) == HIGH) joystickWobbleSpeed = ATTACK_THRESHOLD;
    // in diesem Fall rest der methode  getInput() auskommentieren

    //Holt die Messdaten vom MPU Chip ab
    mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int a = (JOYSTICK_ORIENTATION == 0 ? ax : (JOYSTICK_ORIENTATION == 1 ? ay : az)) / 166;
    int g = (JOYSTICK_ORIENTATION == 0 ? gx : (JOYSTICK_ORIENTATION == 1 ? gy : gz));
    //checkt ob der joystick innerhalb des definierten winkels bleibt
    if (abs(a) < JOYSTICK_DEAD_ANGLE) a = 0;
    if (a > 0) a -= JOYSTICK_DEAD_ANGLE;
    if (a < 0) a += JOYSTICK_DEAD_ANGLE;
    //schreibt werte in RunningMedian object
    MPUTiltInput.add(a);
    MPUWobbleInput.add(g);

    //errechnet stablien median zum weitergeben (aus klasse running median)
    joystickTilt = MPUTiltInput.getMedian();
    //prüft die richtung des joysticks
    if (JOYSTICK_DIRECTION == 1) {
      joystickTilt = 0 - joystickTilt;
    }
    //filtert den höchsten speed wert aus den messdaten
    joystickWobbleSpeed = abs(MPUWobbleInput.getHighest());
    //Serial.println(joystickTilt);
  }

  //methode checkt ob der joystick-speed seinen  threshold erreicht hat
  bool Joystick::isAttacking() {
    return (joystickWobbleSpeed > ATTACK_THRESHOLD);
  }
  //getter für tilt value
  int Joystick::getJoystickTilt() {
    return joystickTilt;
  }

  //getter für speed value
  int Joystick::getJoystickWobble() {
    return joystickWobbleSpeed;
  }
