// **Modul: Joystick Input**
// The joystick can be used as an independent module for input and deployed in other games and projects.
// To do this add the Joystick library (Joystick.cpp & Joystick.h) to your Arduino > Libraries folder.
// The library gives you access to two main input values: joystickTilt and joystickWobbleSpeed.
// **joystickTilt**
// Tilt of the Joystick, which is measured in degrees (int) and takes values of -90 to 90.
// In our game we use this as two parameters joystickTilt < 0 walking backwards and joystickTilt >0 walking forwards.
// The value of the tilt determines the speed of walking: a 90° angle indicates a very tiltet joystick and the player moves very fast.
// At a 10° angle the player moves slow. At 0 it stops moving.
// The tilt is measured from an erect joystick downwards along one axis (in our case back and forth, can be set with JOYSTICK_ORIENTATION).
// **joystickWobbleSpeed**
// How fast the joystick wobbles (if you let the spiral wobble).
// You can either use this variable as a linear function (the faster it wobbles, the more of something happens, or the stronger something appears),
// or in combination with a threshold. If the joystick wobbles faster than the threshold, an action is triggered.
// In our case we use the letter method, and if the joystickWobbleSpeed reaches the threshold our player goes to attack mode.
// For this we implemented the method isAttacking() which returns true if the threshold is reached.

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define JOYSTICK_ORIENTATION 1        // [0,1,2] legt die Orientierung des Joysticks fest (in welche Richtung Steuern, in welche Wackeln)
#define JOYSTICK_DIRECTION   0        // 0/1 Bewegungsrichtung ändern (hoch/runter)
#define ATTACK_THRESHOLD     30000    // Ab welchem Threshold (Geschwindigkeit) des Joysticks wird ein Angriff ausgelöst
#define JOYSTICK_DEAD_ANGLE    5      // Winkel der ignoriert wird

#pragma once
#include "MPU6050.h"
#include "RunningMedian.h"

class Joystick {
  private:
    MPU6050 mpu6050;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int joystickTilt;
    int joystickWobbleSpeed;
    RunningMedian MPUTiltInput = RunningMedian(5);
    RunningMedian MPUWobbleInput = RunningMedian(5);
  public:
    Joystick();
    void getInput();          //Holt messdaten vom MPU6050
    int getJoystickTilt();    //Neigung -> Bewegungsrichtung vor/zurück und wie schnell
    int getJoystickWobble();  //Geschwindigkeit
    bool isAttacking();       // -> nimmt Geschwindigkeit und schaut ob attacke ausgelöst wird oder nicht
};


#endif // JOYSTICK_H
