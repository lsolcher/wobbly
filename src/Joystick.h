#pragma once
#include "MPU6050.h"
#include "RunningMedian.h"

#define JOYSTICK_ORIENTATION 1        // [0,1,2] legt die Orientierung des Joysticks fest (in welche Richtung Steuern, in welche Wackeln)
#define JOYSTICK_DIRECTION   0        // 0/1 Bewegungsrichtung ändern (hoch/runter)
#define ATTACK_THRESHOLD     30000    // Ab welchem Threshold (Geschwindigkeit) des Joysticks wird ein Angriff ausgelöst
#define JOYSTICK_DEAD_ANGLE    5      // Winkel der ignoriert wird TODO

MPU6050 mpu6050;
int16_t ax, ay, az;
int16_t gx, gy, gz;

RunningMedian MPUAngleSamples = RunningMedian(5);
RunningMedian MPUWobbleSamples = RunningMedian(5);

class Joystick {

public:
  static void getInput() {
    // This is respeedonsible for the spieler movement speedeed and angreifend.
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

  static bool isAttacking() {
    return (joystickWobbleSpeed > ATTACK_THRESHOLD);   //methode checkt ob der joystick seinen 
  }

  static int joystickTilt;              // Variable speichert Winkel des Joysticks: <=0  1. input (bei uns: rückwärts gehen) >=0  2. input (bei uns: vorwärts gehen)
  static int joystickWobbleSpeed;       // Variable speichert max. Geschwindigkeit des Joysticks beim wobblen. 
                                        // Als Input zu verwenden entweder Höhe der Geschwindigkeit oder als Abfrage mit Threshold (wobblen) dann via isAttacking() 
};
int Joystick::joystickTilt = 0;             
int Joystick::joystickWobbleSpeed = 0;


