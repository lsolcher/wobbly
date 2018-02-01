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
#include "Trap.h"
#include "Water.h"
#include "LightEffects.h"

//TODO: Wasser, Endgegner, EndAnimation, Sound



MPU6050 mpu6050;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Game setup
#define TICKTIME  20
short currentLevel = 0;
short difficulty = 0; //0 = easy, 1 = hard
bool attacking = false;
short attackTicks = 0;
int attackRange = 2;
short attackTime = 25 ;
short spawnSpeed;
uint8_t trapSize = 1;
int tick = 0;
uint8_t moveDelay = 0;
int lifeLEDs[3] = {28, 42 ,52};
CRGB::HTMLColorCode winColor;

// LED setup
#define LED_NUM             135    // Anzahl LEDs - 1
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

#undef min
inline int min(int a, int b) { return ((a)<(b) ? (a) : (b)); }
inline double min(double a, double b) { return ((a)<(b) ? (a) : (b)); }

#undef max
inline int max(int a, int b) { return ((a)<(b) ? (b) : (a)); }
inline double max(double a, double b) { return ((a)<(b) ? (b) : (a)); }

LightEffects effects;// = new LightEffects();
Player* player = new Player();
std::list<Entity*> *exitList = new std::list<Entity*>; //TODO: all in one list?
std::list<Enemy*> *enemyList = new std::list<Enemy*>;
std::list<Water*> *waterList = new std::list<Water*>;
std::list<SpawnPoint*> *spawnList = new std::list<SpawnPoint*>;
std::list<Trap*> *trapList = new std::list<Trap*>;

//TODO: delete lists + objects in lists

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for connection
  // Fast LED
  effects.setBrightness(50);


  // Life LEDs
  for(int i = 0; i<3; i++){
      pinMode(lifeLEDs[i], OUTPUT);
      digitalWrite(lifeLEDs[i], HIGH);
  }
}

void initialize() {
  currentLevel = 0;
  enemyList->clear();
  spawnList->clear();
  trapList->clear();
  exitList->clear();
  waterList->clear();
  player->setColor(CRGB::Pink);
  player->setPosition(500);
  player->setLives(3);
  Serial.println("INIT");

}


void loop() {
  //LightEffects effects;
  /*effects.clear();
  //tick++;
  effects.addWaterAnimation(mapLed(100), 8, 0, CRGB::Blue);
  effects.addWaterAnimation(mapLed(200), 50, 1, CRGB::Blue);
  effects.addTrapAnimation(mapLed(500), 5, CRGB::Red, CRGB::Orange, tick);
  effects.addPoint(mapLed(900), CRGB::Gold);
  effects.addWaveAnimation(mapLed(900), 4, CRGB::Yellow, CRGB::Cyan);
  //CRGB(189,183,107);

  effects.show();*/
  //effects.showCrippleAnimation(player->getPosition(), 10, CRGB::Red, CRGB::Violet);
  */

  if(state == "INIT") {
    initialize();
    loadLevel();
  }
  if(state == "GAME") {
    if(attacking){
          SFXattacking();
        }else{
          SFXtilt(joystickTilt);
        }
    if (millis() - timeLastInput >= TICKTIME) {
      getInput();
      checkCollision();
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
        tick > 1000 ? tick = 0 : tick++;
        timeLastInput = millis();
      if(!attacking) {
        checkAttack();
      }
      if(attacking) {
        if(attackTicks == 0)
          attacking = false;
        else
          attackTicks--;
      }
      if(!attacking) {
        //Serial.println(joystickTilt);
        player->move(joystickTilt);
      }
      gameTick();
      drawGame();
      }
  }*/
}

void checkAttack() {
  //Serial.println(joystickWobbleSpeed);
//  Serial.println(ATTACK_THRESHOLD);

  if(joystickWobbleSpeed > ATTACK_THRESHOLD) {
    attacking = true;
    attackTicks = attackTime;
  }
}

int checkWater() {
  int speedAdjust = 0;
  for(auto it=waterList->begin();it!=waterList->end();it++) {
    if(mapLed((*it)->getPosition()) == mapLed(player->getPosition())) {
      if((player->getPosition() == (*it)->getPosition()) ||
         (player->getPosition() > (*it)->getPosition() && mapLed(player->getPosition()) - (*it)->getSize() < mapLed((*it)->getPosition()))) {
           return;
      }
    }
  }
}

void checkCollision() {
  //check player attack
  if(attacking) {
    for(auto it=enemyList->begin();it!=enemyList->end();) {
          if((player->getPosition() < (*it)->getPosition() && mapLed(player->getPosition()) + attackRange > mapLed((*it)->getPosition())) ||
             (player->getPosition() > (*it)->getPosition() && mapLed(player->getPosition()) - attackRange < mapLed((*it)->getPosition()))) {
              it = enemyList->erase(it);
              SFXkill();
            return;
          }
        ++it;
    }
  }

  //check trap attack
  for(auto it=trapList->begin();it!=trapList->end();it++) {
    if((*it)->isActive()) {
      // does trap attack player?
      if((player->getPosition() < (*it)->getPosition() && mapLed(player->getPosition()) + trapSize >= mapLed((*it)->getPosition())) ||
         (player->getPosition() > (*it)->getPosition() && mapLed(player->getPosition()) - trapSize <= mapLed((*it)->getPosition()))) {
           handleCollision((*it));
           return;
         }
         //does trap attack enemy?
      for(auto enemy=enemyList->begin();enemy!=enemyList->end();) {
          if(((*enemy)->getPosition() < (*it)->getPosition() && mapLed((*enemy)->getPosition()) + trapSize >= mapLed((*it)->getPosition())) ||
             ((*enemy)->getPosition() > (*it)->getPosition() && mapLed((*enemy)->getPosition()) - trapSize <= mapLed((*it)->getPosition()))) {
               enemy = enemyList->erase(enemy);
               return;
             }
        ++enemy;
      }
    }
  }

  //check enemy attack
  for(auto it=enemyList->begin();it!=enemyList->end();it++) {
    if(mapLed((*it)->getPosition()) == mapLed(player->getPosition())) {
      die();
      return;
    }
  }

  //check if exit reached
  for(auto it=exitList->begin();it!=exitList->end();it++) {
    if(mapLed((*it)->getPosition()) == mapLed(player->getPosition())) {
      handleCollision((*it));
      return;
    }
  }
}


void loadLevel() {
  for(int i = 0; i<3; i++){
     digitalWrite(lifeLEDs[i], player->getLives()>i?HIGH:LOW);
  }
  enemyList->clear();
  spawnList->clear();
  trapList->clear();
  exitList->clear();
  waterList->clear();
  switch(currentLevel) {
    case 0:
      Serial.println("Load Level 0");
      loadStartMenu();
      break;
    case 1:
      Serial.println("Load Level 1");
      loadLevelOne();
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
      loadLevelSeven();
      break;
    case 5:
      Serial.println("Load Level 5");
      loadLevelFour();
      break;
    case 6:
      Serial.println("Load Level 6");
      loadLevelFive();
      break;
    case 7:
      Serial.println("Load Level 7");
      loadLevelSix();
      break;
    case 8:
      Serial.println("Load Level 8");
      loadLevelEight();
      break;
    case 9:
      Serial.println("Load Level 9");
      loadLevelNine();
      break;
    default:
      break;
  }
}

void gameTick() {
  spawnEnemies();
  tickTraps();
  moveEnemies();
}

void tickTraps() {
  for(auto it=trapList->begin();it!=trapList->end();it++) {
      (*it)->setActive(tick);
  }
}


void moveEnemies() {
  for(auto it=enemyList->begin();it!=enemyList->end();it++) {
    if((*it)->getType() == "enemy") {
      (*it)->move(player->getPosition(), tick);
      //(*it)->setPosition(enemy->getPosition());
    }
  }
}

void spawnEnemies() {
  for(auto it=spawnList->begin();it!=spawnList->end();it++) {
    if((*it)->spawns()) {
      Enemy* e = new Enemy((*it)->getPosition(), (*it)->getSpawnColor(), "enemy");
      e->setChangesDirection(false);
      enemyList->push_back(e);
    }
  }
}

void setDifficultySettings() {
  Serial.println("DIFFICULTY");
  if(difficulty == 0) {
    trapSize = 3;
    attackRange = 7;
    attackTime = 30;
    spawnSpeed = 175;
    winColor = CRGB::Silver;
  } else {
    trapSize = 5;
    attackRange = 4;
    attackTime = 15;
    spawnSpeed = 125;
    winColor = CRGB::Gold;
  }
}

void handleCollision(Entity* collObj) {
  Serial.println(collObj->getType());
  if(collObj->getType() == "easy") {
    SFXwin();
    difficulty = 0;
    currentLevel = 1;
    player->setLives(3);
    setDifficultySettings();
    showWinAnimation();
    loadLevel();
  } else if(collObj->getType() == "hard") {
    SFXwin();
    difficulty = 1;
    currentLevel = 1;
    player->setLives(3);
    setDifficultySettings();
    showWinAnimation();
    loadLevel();
  } else if(collObj->getType() == "exit") {
    if(currentLevel == 9) {
      if(enemyList->empty()) {
        SFXcomplete();
        showWinAnimation();
        showWinAnimation();
        showWinAnimation();
        initialize();
      }
    } else {
      SFXwin();
      player->setLives(3);
      currentLevel++;
      showWinAnimation();
      loadLevel();
    }
  } else if(collObj->getType() == "enemy" || collObj->getType() == "trap") {
    die();
  }
}



void drawGame() {
  showLeds();
}

void die(){
  SFXdead();
  player->setLives(player->getLives() - 1);
  for(int i = 0; i<3; i++){
     digitalWrite(lifeLEDs[i], player->getLives()>i?HIGH:LOW);
  }
  showDieAnimation();
  if(player->getLives() == 0)
    gameOver();
  else {
    loadLevel();
  }
}

void showDieAnimation() {
  effects.showCrippleAnimation(player->getPosition(), 20, CRGB::Red, CRGB::Violet);
}

void showWinAnimation() {
  effects.showSnakeAnimation(1, winColor);
}

void gameOver() {
  state = "INIT";
}

void loadStartMenu() {
  Entity* easy = new Entity(WORLD_MIN, CRGB::LightGreen, "easy");
  Entity* hard = new Entity(WORLD_MAX, CRGB::DarkRed, "hard");
  exitList->push_back(easy);
  exitList->push_back(hard);
  state = "GAME";
}

void loadLevelOne() {
  player->setPosition(WORLD_MIN);
  Enemy* e = new Entity(WORLD_MAX/2, CRGB::Blue, "enemy");
  e->setChangesDirection(false);
  enemyList->push_back(e);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
  state = "GAME";
}

void loadLevelTwo() {
  player->setPosition(WORLD_MIN);
  SpawnPoint* sp = new Entity(WORLD_MAX, CRGB::Red, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}

void loadLevelThree() {
  player->setPosition(WORLD_MAX / 2);
  Enemy* e = new Enemy(WORLD_MAX, CRGB::Blue, "enemy");
  enemyList->push_back(e);
  Enemy* e2 = new Enemy(WORLD_MIN, CRGB::Blue, "enemy");
  enemyList->push_back(e2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}

void loadLevelFour() {
  player->setPosition(WORLD_MIN);
  Enemy* e = new Enemy(WORLD_MAX, CRGB::Blue, "enemy");
  e->setChangesDirection(true);
  enemyList->push_back(e);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}


void loadLevelFive() {
  player->setPosition(WORLD_MAX / 2);
  SpawnPoint* sp = new SpawnPoint(WORLD_MAX-10, CRGB::DarkRed, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp);
  SpawnPoint* sp2 = new SpawnPoint(WORLD_MIN, CRGB::DarkRed, "spawn");
  sp2->setSpawnSpeed(spawnSpeed - 50);
  sp2->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}


void loadLevelSix() {
  player->setPosition(WORLD_MAX / 2);
  SpawnPoint* sp = new SpawnPoint(WORLD_MAX-10, CRGB::DarkRed, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp);
  SpawnPoint* sp2 = new SpawnPoint(WORLD_MIN, CRGB::DarkGray, "spawn");
  sp2->setSpawnSpeed(spawnSpeed - 50);
  sp2->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp2);
  Enemy* e = new Entity(WORLD_MAX, CRGB::Blue, "enemy");
  e->setChangesDirection(true);
  enemyList->push_back(e);
  Enemy* e2 = new Entity(WORLD_MIN, CRGB::Blue, "enemy");
  e2->setChangesDirection(true);
  enemyList->push_back(e2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}

void loadLevelSeven() {
  player->setPosition(WORLD_MIN);
  Enemy* e = new Enemy(WORLD_MAX, CRGB::Blue, "enemy");
  e->setChangesDirection(false);
  enemyList->push_back(e);
  Trap* t = new Trap(WORLD_MAX/2, CRGB::DarkGray, "trap");
  t->setInactiveColor(CRGB::Orange);
  t->setActiveColor(CRGB::Red);
  t->setSize(trapSize);
  trapList->push_back(t);
  Trap* t2 = new Trap(WORLD_MAX * 0.75, CRGB::DarkGray, "trap");
  t2->setInactiveColor(CRGB::Orange);
  t2->setActiveColor(CRGB::Red);
  t2->setSize(trapSize);
  trapList->push_back(t2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}

void loadLevelEight() {
  player->setPosition(WORLD_MIN);
  SpawnPoint* sp = new SpawnPoint(WORLD_MAX-10, CRGB::DarkRed, "spawn");
  sp->setSpawnSpeed(spawnSpeed);
  sp->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp);
  SpawnPoint* sp2 = new SpawnPoint(WORLD_MIN, CRGB::DarkRed, "spawn");
  sp2->setSpawnSpeed(spawnSpeed + 200);
  sp2->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp2);
  Trap* t = new Trap(WORLD_MAX/4, CRGB::DarkGray, "trap");
  t->setInactiveColor(CRGB::Orange);
  t->setActiveColor(CRGB::Red);
  t->setSize(trapSize);
  trapList->push_back(t);
  Trap* t2 = new Trap(WORLD_MAX/3, CRGB::DarkGray, "trap");
  t2->setInactiveColor(CRGB::Orange);
  t2->setActiveColor(CRGB::Red);
  t2->setSize(trapSize);
  trapList->push_back(t2);
  Trap* t3 = new Trap(WORLD_MAX/2, CRGB::DarkGray, "trap");
  t3->setInactiveColor(CRGB::Orange);
  t3->setActiveColor(CRGB::Red);
  t3->setSize(trapSize);
  trapList->push_back(t3);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  enemyList->push_back(exit);
}

void loadLevelNine() {
  player->setPosition(WORLD_MAX / 2);
  SpawnPoint* sp = new SpawnPoint(WORLD_MAX-10, CRGB::DarkRed, "spawn");
  sp->setSpawnSpeed(spawnSpeed - 25);
  sp->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp);
  SpawnPoint* sp2 = new SpawnPoint(WORLD_MIN, CRGB::DarkGray, "spawn");
  sp2->setSpawnSpeed(spawnSpeed - 75);
  sp2->setSpawnColor(CRGB::Blue);
  spawnList->push_back(sp2);
  Enemy* e = new Enemy(WORLD_MIN, CRGB::Blue, "enemy");
  e->setChangesDirection(true);
  e->setMoves(false);
  enemyList->push_back(e);
  Enemy* e2 = new Enemy(WORLD_MAX-10, CRGB::Blue, "enemy");
  e2->setChangesDirection(true);
  e2->setMoves(false);
  enemyList->push_back(e2);
  Entity* exit = new Entity(WORLD_MAX, CRGB::Gold, "exit");
  exitList->push_back(exit);
}



void showLeds() {
  effects.clear();
  showWater();
  showSpawners();
  showExits();
  showTraps();
  showEnemies();
  showPlayer();
  effects.show();
}

void showWater() {
  for(auto it=waterList->begin();it!=waterList->end();it++) {
      for(int i = 0; i <= (*it)->getSize(); i++) {
        if((*it)->getDirection() == 1) {
          effects.addWaterAnimation(mapLed((*it)->getPosition()), 4, 1, (*it)->getColor());
        } else {
          effects.addWaterAnimation(mapLed((*it)->getPosition()), 4, 0, (*it)->getColor());
        }
    }
  }
}

void showExits() {
  for(auto it=exitList->begin();it!=exitList->end();it++) {
    effects.addPoint(mapLed((*it)->getPosition()), (*it)->getColor());
  }
}

void showTraps() {
  for(auto it=trapList->begin();it!=trapList->end();it++) {
    effects.addTrapAnimation(mapLed((*it)->getPosition()), 4, (*it)->getColor(), tick, (*it)->isActive());
  }
}

void showSpawners() {
  for(auto it=spawnList->begin();it!=spawnList->end();it++) {
    effects.addPoint(mapLed((*it)->getPosition()), (*it)->getColor());
  }
}

void showEnemies() {
  for(auto it=enemyList->begin();it!=enemyList->end();it++) {
    effects.addPoint(mapLed((*it)->getPosition()), (*it)->getColor());
  }
}

void showPlayer() {
  effects.addPoint(mapLed(player->getPosition()), player->getColor());
  if(attacking && tick % 4 > 2) {
    effects.addWaveAnimation(player->getPosition(), 4, CRGB::Yellow, CRGB::Cyan);
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



// ---------------------------------
// -------------- SFX --------------
// ---------------------------------
void SFXtilt(int amount){
    int f = map(abs(amount), 0, 90, 80, 900)+random8(100);
    //if(playerPositionModifier < 0) f -= 500;
    //if(playerPositionModifier > 0) f += 200;
    toneAC(f, min(min(abs(amount)/9, 5), 10));

}
void SFXattacking(){
    int freq = map(sin(millis()/2.0)*1000.0, -1000, 1000, 500, 600);
    if(random8(5)== 0){
      freq *= 3;
    }
    toneAC(freq, 10);
}
void SFXdead(){
    int freq = max(1000 - (millis()-700), 10);
    freq += random8(200);
    int vol = max(10 - 700/200, 0);
    toneAC(freq, 10);
}
void SFXkill(){
    toneAC(2000, 10, 1000, true);
}
void SFXwin(){
    int freq = (1000)/3.0;
    freq += map(sin(millis()/20.0)*1000.0, -1000, 1000, 0, 20);
    int vol = 10;//max(10 - (millis()-stageStartTime)/200, 0);
    toneAC(freq, 10);
}

void SFXcomplete(){
    noToneAC();
}
