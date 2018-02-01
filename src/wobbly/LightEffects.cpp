#include "LightEffects.h"


LightEffects::LightEffects() {
  FastLED.addLeds<CHIPSET, DATAPIN, COLORCODE>(leds, NUM_LED);
  FastLED.setBrightness(_brightness);
  FastLED.setDither(1);
}

void LightEffects::clear() {
  FastLED.clear();
}

void LightEffects::show() {
  FastLED.show();
}

void LightEffects::setBrightness(uint16_t brightness) {
  _brightness = brightness;
}

void LightEffects::setAnimationSpeed(uint8_t animationSpeed) {
  _animationSpeed = animationSpeed;
}

void LightEffects::addPoint(uint16_t origin,CRGB::HTMLColorCode color) {
  if(origin < NUM_LED)
    leds[origin] = color;
}

void LightEffects::addWaveAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode endWaveColor, CRGB::HTMLColorCode middleWaveColor) {
  for (uint16_t i = 1; i < size; i++) {
    CRGB color;
    if (i == size - 1)
      color = endWaveColor;
    else
      color = middleWaveColor;
      if(origin+i < NUM_LED) {
        leds[origin+i] = color;
      }
      if(origin-i > 0) {
        leds[origin-i] = color;
      }
  }
}

void LightEffects::addTrapAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode activeColor, CRGB::HTMLColorCode inactiveColor, int tick) {
  bool active;
  CRGB::HTMLColorCode currentColor;
  (tick % 200 > 100) ? active = true : active = false;
  (active == true) ? currentColor = activeColor : currentColor = inactiveColor;
  for(uint16_t i = 0; i <= size; i++) {
    if(!active || tick % 4 > 2) {
      leds[origin] = currentColor;
      for(uint16_t i = 1; i < size+1; i++) {
        if(origin+i < NUM_LED)
          leds[origin+i] = currentColor;
        if(origin-i > 0)
          leds[origin-i] = currentColor;
      }
    }
  }
}

void LightEffects::addTrapAnimation(uint16_t origin, uint16_t size, CRGB::HTMLColorCode color, int tick, bool active) {
  for(uint16_t i = 0; i <= size; i++) {
    if(!active || tick % 4 > 2) {
      leds[origin] = color;
      for(uint16_t i = 1; i < size+1; i++) {
        if(origin+i < NUM_LED)
          leds[origin+i] = color;
        if(origin-i > 0)
          leds[origin-i] = color;
      }
    }
  }
}


void LightEffects::addWaterAnimation(uint16_t origin, uint16_t size, bool direction, CRGB::HTMLColorCode color) {
    uint16_t tick = (100*size+millis()) % (100*size);
    for(uint16_t i = 0; i <= size; i++) {
      if((origin+i) < NUM_LED) {
        // moving up
        if(direction == 1) {
          if(i == 0)
            leds[origin] = color;
          else if(tick > i*50) {
            if(origin + i < NUM_LED)
              leds[origin + i] = color;
          }
          //moving down
        } else {
          if(i == size)
            leds[origin + i] = color;
          else if(tick > i*50) {
            if(origin + i < NUM_LED)
              leds[origin + size - i] = color;
          }
        }
    }
  }
}

void LightEffects::showCrippleAnimation(int center, uint16_t steps, CRGB::HTMLColorCode crippleColor, CRGB::HTMLColorCode bgColor) {
  FastLED.clear();
  for(uint16_t l = 0; l < NUM_LED; l++) {
    leds[l] = CHSV(bgColor, 255, 50);
  }
  for(uint16_t step = 0; step < steps; step++) {
    leds[wrap(center + step)] = CHSV(crippleColor, 255, pow(0.8, step)*255);
    leds[wrap(center + step)] = CHSV(crippleColor, 255, pow(0.8, step)*255);
    if (step > 3) {
      leds[wrap(center + step + 3)] = CHSV(crippleColor, 255, pow(0.8, step - 2)*255);
      leds[wrap(center - step + 3)] = CHSV(crippleColor, 255, pow(0.8, step - 2)*255);
    }
    FastLED.show();
    delay(50);
  }
}

void LightEffects::showSnakeAnimation(bool direction, CRGB::HTMLColorCode color) {
  FastLED.clear();
  if(direction == 1) {
    for(int i = NUM_LED - 1; i > 0; i--) {
      leds[i] = color;
      FastLED.show();
      delay(5);
    }
  }
  else {
    for(int i = 0; i < NUM_LED; i++) {
      leds[i] = color;
      FastLED.show();
      delay(5);
    }
  }
}


uint16_t LightEffects::wrap(uint16_t step) {
  if(step < 0) return NUM_LED + step;
  if(step > NUM_LED - 1) return step - NUM_LED;
  return step;
}
