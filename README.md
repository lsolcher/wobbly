#WOOBLI
---
## One Dimensional Arcade Game

For images of the game and build process please refer to []()

### Components needed
- Arduino Mega
- [MPU6050](https://maker.pro/education/imu-interfacing-tutorial-get-started-with-arduino-and-the-mpu-6050-sensor) (Gyroscope & Accelerometer)
- WS2812B (LED Strip(s), we used 144LED/m, [datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf))
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
- I2CDev

## How-To
- Download the Arduino IDE
- Clone this repository to your computer
- Copy all libraries from the lib folder of the repo to your Arduino *Libraries* folder
- Connect all the components

### Connections
- MPU6050 see [this diagram](https://3.bp.blogspot.com/-oZgRb23Jj2A/U8FoKkbt1mI/AAAAAAAAAJ8/FTjt2dNlI_o/s1600/5.jpg)
- WS2812B the data wire (usually green) to DataPin 6, the GND (black or white) cable to GND
**Electricity**the red (+) and black (-/gnd) wire should be connected to an external power source, because excessiv power could damage your arduino board.
calculate like this: for each of your LEDs add 20mA (each LED has a maximum power of 60mA when all three rgb lights (20mA/each) are on full brightness (=white). however, you will barely ever use all your LEDs at full power, so calculating with 20-30mA per LED will serve most purposes). In our case we had 288 LEDs x 20mA = ca. 6A
Note that a higher Amper number doesn't hurt, whereas a higher Voltage number is bad. Stick to 5V and experiment with the Ampers. In our case 2A where enough in the end.
**Important:** you need to connect one (-/gnd) cable to the arduino as well, because the data pin will need a closed circuit!
- Life LEDs connect a 1K resistor and cable to the longer tip (+) of the LED and put them individually into DataPins


- Open src > wobbly > wobbly.ino with the Arduino IDE.
- In the IDE set Tools > Board and Port to your Arduino Mega board and upload the sketch (and all other classes in the folder)
- WOOBLI should now be running on your Arduino

### Troubleshooting
If you encounter problems running the sketch, try checking if all cables are in the correct sockets and/or try reconnecting them.
You can check the functionality of individual components with example-sketches from the libraries.

You could e.g. use File > Examples
\> MPU6050 > MPU6050_DMG6 (load to arduino, open serial monitor, set to 115200 bd, see input?) to test the functionality of the MPU
\> FastLED > Blink (then select the WS2812B strip from the list and upload sketch, LED strip blinks?) to test the functionality of the LED strips
\> Digital > Melody (load to arduino, hear sound?) to select the funtionality of the Speaker
\> Basic > Blink (load to arduino, see blinking?) to test functionality of the board

For working on the code with a little bit more comfortable IDE (autocomplete, refactoring) you can use Visual Studio Code with an Arduino plugin. See [here](https://daniel-ziegler.com/arduino/mikrocontroller/ide/2017/07/10/Microsoft-Visual-Studio-Code-alternative-Arduino-IDE/) how to set it up.



## Modules
### 1. Joystick Input
### 2. Animation Output
