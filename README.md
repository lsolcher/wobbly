#WOOBLI
---
## One Dimensional Arcade Game

For images of the game and build process please refer to [our PDF presentation]().

Woobli is a one dimensional arcade game, operated by a joystick that is a spiral. The playing field is an LED strip (one dimensional array). The player starts at one end of the array and needs to move to the opposite exit. The player is operated by a joystick and can walk forwards and backwards, speed and direction determined by the tilt of the joystick. On the way she will encounter obstacles such as lava and water, and also enemies. To survive an encounter with an enemy, the player needs to attack it by wobbling the joystick-spiral really fast. When a certain threshold is reached the player will go into attack mode, become larger and kill all enemies in reach.

### Components needed
- Arduino Mega
- [MPU6050](https://maker.pro/education/imu-interfacing-tutorial-get-started-with-arduino-and-the-mpu-6050-sensor) (Gyroscope & Accelerometer)
- WS2812B (LED Strip(s), we used 2x144LED/m, you can use as many as <ou want. [datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf))
- 8 Ohm Speaker
- 3 standard LEDs (to indicate lives left)
- Cables and 1K Resistors
- One door-stopper spring (for the wobble joystick)
- 3D printed case and joystick head (files attached)
- 5V/2A Charger (Amper number dependent on number of LEDs)

### Libraries needed (included in lib folder)
- FastLED - for the LED strips
- MPU6050 - to measure the Joystick tilt and speed
- toneAC - for sound effects
- RunningMedian - to make some calculations for speed measurements
- I2CDev - provides interface classes for devices

## How-To
- Download the Arduino IDE
- Clone this repository to your computer
- Copy all libraries from the lib folder of the repo to your Arduino *Libraries* folder
- Connect all the components

### Connections
For a first trial its advisable to use a breadboard.

- MPU6050: see [this diagram](https://3.bp.blogspot.com/-oZgRb23Jj2A/U8FoKkbt1mI/AAAAAAAAAJ8/FTjt2dNlI_o/s1600/5.jpg)
- WS2812B: the data wire (usually green) to DataPin 6, the GND (black or white) cable to GND
**Electricity**the red (+) and black (-/gnd) wire should be connected to an external power source, because excessiv power could damage your arduino board.
calculate like this: for each of your LEDs add 20mA (each LED has a maximum power of 60mA when all three rgb lights (20mA/each) are on full brightness (=white). however, you will barely ever use all your LEDs at full power, so calculating with 20-30mA per LED will serve most purposes). In our case we had 288 LEDs x 20mA = ca. 6A
Note that a higher Amper number doesn't hurt, whereas a higher Voltage number is bad. Stick to 5V and experiment with the Ampers. In our case 2A where enough in the end.
**Important:** you need to connect one (-/gnd) cable to the arduino as well, because the data pin will need a closed circuit!
- Life LEDs: connect a 1K resistor and cable to the longer tip (+) of the LED and put them individually into DataPins 28, 42  and 52 (you can use any free DataPin, but would need to change the declaration in the sketch), connevct the shorter legs to GND.


- Open src > wobbly > wobbly.ino with the Arduino IDE.
- In the IDE set Tools > Board and Port to your Arduino Mega board and upload the sketch (including all other classes in the folder wobbly)
- Congrats, WOOBLI should now be running on your Arduino and you shouls be able to navigate by moving the MPU6050 around.

### Troubleshooting
If you encounter problems running the sketch, try checking if all cables are in the correct sockets and/or try reconnecting them.
You can check the functionality of individual components with example-sketches from the libraries.

You could e.g. use File > Examples
\> MPU6050 > MPU6050_DMG6 (load to arduino, open serial monitor, set to 115200 bd, see input?) to test the functionality of the MPU
\> FastLED > Blink (then select the WS2812B strip from the list and upload sketch, LED strip blinks?) to test the functionality of the LED strips
\> Digital > Melody (load to arduino, hear sound?) to select the funtionality of the Speaker
\> Basic > Blink (load to arduino, see blinking?) to test functionality of the board

For working on the code with a little bit more comfortable IDE (autocomplete, refactoring) you can use Visual Studio Code with an Arduino plugin. See [here](https://daniel-ziegler.com/arduino/mikrocontroller/ide/2017/07/10/Microsoft-Visual-Studio-Code-alternative-Arduino-IDE/) how to set it up.

##Game Components
### Player
### Enemies
### Traps
### Water
### Lava

## Modules
### 1. Joystick Input
The joystick can be used as an independent module for input and deployed in other games and projects.
To do this add the Joystick library (Joystick.cpp & Joystick.h) to your Arduino > Libraries folder.
The library gives you access to two main input values: joystickTilt and joystickWobbleSpeed.
#### joystickTilt
Tilt of the Joystick, which is measured in degrees and has a value of -90 to 90.
In our game we use this as two parameters joystickTilt < 0 walking backwards and joystickTilt >0 walking forwards.
The value of the tilt determines the speed of walking: a 90° angle indicates a very tiltet joystick and the player moves very fast. At a 10° angle the player moves slow.
The tilt is measured from an erect joystick downwards along one axis (in our case back and forth, can be set with JOYSTICK_ORIENTATION).
#### joystickWobbleSpeed
How fast the joystick wobbles (if you let the spiral wobble).
You can either use this variable as a linear function (the faster it wobbles, the more of something happens, or the stronger something appears), or in combination with a threshold. If the joystick wobbles faster than the threshold, an action is triggered.
In our case we use the letter method, and if the joystickWobbleSpeed reaches the threshold our player goes to attack mode. For this we implemented the method isAttacking() which returns true if the threshold is reached.

### 2. Animation Output
