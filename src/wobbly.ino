// Libs
#pragma once
#include "FastLED.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "toneAC.h"
#include "iSin.h"
#include "RunningMedian.h"
#include "ArduinoSTL.h"
#include <list>
#include "Player.h"
#include "Enemy.h"
#include "SpawnPoint.h"




MPU6050 mpu6050;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Game setup
#define TICKTIME  10
short currentLevel = 0;
short difficulty = 0; //0 = easy, 1 = hard
bool attacking = false;
short attackTicks = 0;
int attackRange = 0;
short attackTime = 5;
short spawnSpeed;
int tick = 0;

// LED setup
#define LED_NUM             100    // Anzahl LEDs
#define WORLD_MIN             0 // min world index
#define WORLD_MAX            999 //max world index
#define DATA_PIN             6     // Daten Pin für den LED Schlauch am Arduino
#define BRIGHTNESS           50
#define DIRECTION            1     // 0 = oben nach unten, 1 = unten nach oben
#define MIN_FRAME_INTERVAL  16    // Min interval bevor neuer Frame gezeichnet wird  33ms = 30fps / 16ms = 63fps
#define GRAVITY              1     // 0/1 Schwerkraft benutzen (LED Streifen geht an der Wand hoch: 1)



// Joystick

#define JOYSTICK_ORIENTATION 1     // [0,1,2] legt die Orientierung des Joysticks fest (in welche Richtung Steuern, in welche Wackeln)
#define JOYSTICK_DIRECTION   0      // 0/1 Bewegungsrichtung ändern (hoch/runter)
#define ATTACK_THRESHOLD     30000 // Ab welchem Threshold (Geschwindigkeit) des Joysticks wird ein Angriff ausgelöst
#define JOYSTICK_DEAD_ANGLE    5     // Winkel der ignoriert wird TODO
int joystickTilt = 0;              // Variable speichert Winkel des Joysticks
int joystickWobbleSpeed = 0;            // Variable speichert max. Geschwindigkeit des Joysticks
char* state = "INIT";
long timeLastFrame = 0;
long timeLastInput = 0;
#define TIMEOUT              30000


CRGB leds[LED_NUM];
RunningMedian MPUAngleSamples = RunningMedian(5);
RunningMedian MPUWobbleSamples = RunningMedian(5);



Player* player = new Player();
std::list<Entity*> *entityList = new std::list<Entity*>;
std::list<SpawnPoint*> *spawnList = new std::list<SpawnPoint*>;
std::list<Entity*>::iterator it;


void setup() {

  Serial.begin(9600);
  while (!Serial); // wait for connection
  // Fast LED
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setDither(1);
}

void initialize() {
  currentLevel = 0;
  entityList->clear();
  spawnList->clear();
  player->setColor(CRGB::Green);
  player->setPosition(500);
  player->setLives(3);
  Serial.println("INIT");
}


void loop() {
  tick > 100 ? tick = 0 : tick++;
  if (millis() - timeLastInput >= TICKTIME) {
    getInput();
    if(abs(joystickTilt) > JOYSTICK_DEAD_ANGLE){
        timeLastInput = millis();
        if(state == "SCREENSAVER"){
            loadStartMenu();
            state = "GAME";
        }
    }else{
        if(timeLastInput+TIMEOUT < millis()){
            state = "SCREENSAVER";
        }
    }
  }
  if(state == "INIT") {
    initialize();
    loadLevel();
  }
  if(state == "GAME") {
    if(!attacking)
      checkAttack();
    if(attacking) {
      if(--attackTicks == 0)
      attacking = false;
    }
    checkCollision();
    getInput();
    if(!attacking)
      player->move(joystickTilt);
    gameTick();
    drawGame();
  }
}

void checkAttack() {
  if(joystickWobbleSpeed > ATTACK_THRESHOLD) {
    attacking = true;
    attackTicks = attackTime;
  }
}

void checkCollision() {
  if(attacking) {
    for(it=entityList->begin();it!=entityList->end();) {
      if((*it)->getType() == "enemy") {
          if(mapLed(player->getPosition() + attackRange) > mapLed((*it)->getPosition())) {
          /*  Serial.println(mapLed(player->getPosition()));
            Serial.println(mapLed(player->getPosition() + attackRange));
            Serial.println(mapLed((*it)->getPosition()));
            Serial.println(mapLed((*it)->getPosition() - attackRange));*/
            it = entityList->erase(it);
            return;
          }
        ++it;
      } else
        ++it;
    }
  }
  for(it=entityList->begin();it!=entityList->end();it++) {
    if(mapLed((*it)->getPosition()) == mapLed(player->getPosition())) {
      handleCollision((*it));
      return;
    }
  }
}


void loadLevel() {
  entityList->clear();
  spawnList->clear();
  switch(currentLevel) {
    case 0:
      Serial.println("Load Level 0");
      loadStartMenu();
      break;
    case 1:
      Serial.println("Load Level 1");
      loadLevelFive();
      break;
    case 2:
      Serial.println("Load Level 2");
      loadLevelTwo();
      break;
    case 3:
      Serial.println("Load Level 3");
      loadLevelThree();
      break;
    case 4:
      Serial.println("Load Level 4");
      loadLevelFour();
      break;
    case 5:
      Serial.println("Load Level 5");
      loadLevelFour();
      break;
    default:
      break;
  }
}

void gameTick() {
  spawnEnemies();
  moveEnemies();
}

void moveEnemies() {
  for(it=entityList->begin();it!=entityList->end();it++) {
    if((*it)->getType() == "enemy") {
      Enemy* enemy = (*it);
      enemy->move(player->getPosition(), tick);
      (*it)->setPosition(enemy->getPosition());
    }
  }
}

void spawnEnemies() {
  for(auto it=spawnList->begin();it!=spawnList->end();it++) {
    if((*it)->spawns()) {
      Enemy* e = new Entity((*it)->getPosition(), (*it)->getSpawnColor(), "enemy");
      entityList->push_back(e);
    }
  }
}

void setDifficultySettings() {
  if(difficulty == 0) {
    attackRange = 100;
    attackTime = 5;
    spawnSpeed = 400;
  } else {
    attackRange = 50;
    attackTime = 2;
    spawnSpeed = 200;
  }
}

void handleCollision(Entity* collObj) {
  if(collObj->getType() == "easy") {
    difficulty = 0;
    currentLevel = 1;
    setDifficultySettings();
    loadLevel();
  } else if(collObj->getType() == "hard") {
    difficulty = 1;
    currentLevel = 1;
    setDifficultySettings();
    loadLevel();
  } else if(collObj->getType() == "exit") {
    currentLevel++;
    loadLevel();
  } else if(collObj->getType() == "enemy") {
    die();
  }
}

void drawGame() {
  showLeds();
}

void die(){
  player->setLives(player->getLives() - 1);
  if(player->getLives() == 0)
    gameOver();
  else {
    showDieAnimation();
    loadLevel();
  }
}

void showDieAnimation() {
  
}

void gameOver() {
  state = "INIT";
}

void loadStartMenu() {
  Entity* easy = new Entity(WORLD_MIN, CRGB::LightGreen, "easy");
  Entity* hard = new Entity(WORLD_MAX, CRGB::DarkRed, "hard");
  entityList->push_back(easy);
  entityList->push_back(hard);
  state = "GAME";
}

void loadLevelOne() {
  player->setPosition(WORLD_MIN);
  Enemy* e = new Entity(WORLD_MAX, CRGB::Red, "enemy");
  entityList->push_back(e);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  entityList->push_back(exit);
}

void loadLevelTwo() {
  player->setPosition(WORLD_MIN);
  SpawnPoint* sp = new Entity(WORLD_MAX, CRGB::Red, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Gray);
  spawnList->push_back(sp);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  entityList->push_back(exit);
}

void loadLevelThree() {
  player->setPosition(WORLD_MAX / 2);
  Enemy* e = new Entity(WORLD_MAX, CRGB::Red, "enemy");
  entityList->push_back(e);
  Enemy* e2 = new Entity(WORLD_MIN, CRGB::Red, "enemy");
  entityList->push_back(e2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  entityList->push_back(exit);
}

void loadLevelFour() {
  player->setPosition(WORLD_MIN);
  Enemy* e = new Entity(WORLD_MAX, CRGB::Red, "enemy");
  e->setChangesDirection(true);
  entityList->push_back(e);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  entityList->push_back(exit);
}


void loadLevelFive() {
  player->setPosition(WORLD_MAX / 2);
  SpawnPoint* sp = new Entity(WORLD_MAX-10, CRGB::DarkRed, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Red);
  spawnList->push_back(sp);
  SpawnPoint* sp2 = new Entity(WORLD_MIN, CRGB::DarkGray, "spawn");
  sp2->setSpawnSpeed(spawnSpeed);
  sp2->setSpawnColor(CRGB::Gray);
  spawnList->push_back(sp2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  entityList->push_back(exit);
}



void showLeds() {
  FastLED.clear();
  for(it=entityList->begin();it!=entityList->end();it++) {
    leds[mapLed((*it)->getPosition())] = (*it)->getColor();
  }
  for(auto it=spawnList->begin();it!=spawnList->end();it++) {
    leds[mapLed((*it)->getPosition())] = (*it)->getColor();
  }
  showPlayer();
  FastLED.show();
}

void showPlayer() {
  leds[mapLed(player->getPosition())] = player->getColor();
  if(attacking) {
    for(int i = 1; i < attackRange; i++) {
        leds[mapLed(player->getPosition()+i)] = CRGB::White;
        leds[mapLed(player->getPosition()-i)] = CRGB::White;
    }
  }
}


int mapLed(int position) {
  return constrain((int)map(position, 0, 1000, 0, LED_NUM-1), 0, LED_NUM-1);
}


// ---------------------------------
// ----------- JOYSTICK ------------
// ---------------------------------
void getInput() {
    // This is respeedonsible for the spieler movement speedeed and angreifend.
    // You can replace it with anything you want that passes a -90>+90 value to joystickTilt
    // and any value to joystickWackelSpeed that is greater than ANGRIFF_THRESHOLD (defined at start)
    // For example you could use 3 momentery buttons:
        // if(digitalRead(leftButtonPinNumber) == HIGH) joystickTilt = -90;
        // if(digitalRead(rightButtonPinNumber) == HIGH) joystickTilt = 90;
        // if(digitalRead(attackButtonPinNumber) == HIGH) joystickWackelSpeed = ANGRIFF_THRESHOLD;

    mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int a = (JOYSTICK_ORIENTATION == 0?ax:(JOYSTICK_ORIENTATION == 1?ay:az))/166;
    int g = (JOYSTICK_ORIENTATION == 0?gx:(JOYSTICK_ORIENTATION == 1?gy:gz));
    if(abs(a) < JOYSTICK_DEAD_ANGLE) a = 0;
    if(a > 0) a -= JOYSTICK_DEAD_ANGLE;
    if(a < 0) a += JOYSTICK_DEAD_ANGLE;
    MPUAngleSamples.add(a);
    MPUWobbleSamples.add(g);

    joystickTilt = MPUAngleSamples.getMedian();
    if(JOYSTICK_DIRECTION == 1) {
        joystickTilt = 0-joystickTilt;
    }
    joystickWobbleSpeed = abs(MPUWobbleSamples.getHighest());
    //Serial.println(joystickTilt);
}
