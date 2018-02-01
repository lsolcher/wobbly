#ifndef LIGHTEFFECTS_H
#define LIGHTEFFECTS_H

#define NUM_LED 135
#define CHIPSET WS2812
#define DATAPIN 6
#define COLORCODE GRB
#define ANIMATIONSPEED 3


#pragma once
#include "Arduino.h"
#include "FastLED.h"
#include <memory>

class LightEffects {
  private:
    uint8_t _animationSpeed = 3;
    int _brightness = 50;
    CRGB leds[NUM_LED];
    uint16_t wrap(uint16_t step);
  public:
    // constructor
    LightEffects();
    //LightEffects();
    void setBrightness(uint16_t brightness);
    void setAnimationSpeed(uint8_t animationSpeed);
    void clear();
    void show();
    void addWaterAnimation(uint16_t origin, uint16_t size, bool direction, CRGB::HTMLColorCode color);
    void addTrapAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode activeColor, CRGB::HTMLColorCode inactiveColor, int tick);
    void addTrapAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode color, int tick, bool active);
    void addPoint(uint16_t origin,CRGB::HTMLColorCode color);
    void addWaveAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode endWaveColor, CRGB::HTMLColorCode middleWaveColor);
    void showSnakeAnimation(bool direction, CRGB::HTMLColorCode Color);
    void showCrippleAnimation(int center, int steps, CRGB::HTMLColorCode crippleColor, CRGB::HTMLColorCode bgColor);
};


#endif // LIGHTEFFECTS_H
