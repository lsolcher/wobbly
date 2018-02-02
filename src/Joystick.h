#ifndef JOYSTICK_H
#define JOYSTICK_H

#define JOYSTICK_ORIENTATION 1        // [0,1,2] legt die Orientierung des Joysticks fest (in welche Richtung Steuern, in welche Wackeln)
#define JOYSTICK_DIRECTION   0        // 0/1 Bewegungsrichtung ändern (hoch/runter)
#define ATTACK_THRESHOLD     30000    // Ab welchem Threshold (Geschwindigkeit) des Joysticks wird ein Angriff ausgelöst
#define JOYSTICK_DEAD_ANGLE    5      // Winkel der ignoriert wird TODO

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
    void getInput();
    int getJoystickTilt();
    int getJoystickWobble();
    bool isAttacking();
};


#endif // JOYSTICK_H
