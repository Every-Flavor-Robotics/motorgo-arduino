# motorgo-arduino

The software to use the MotorGo is split into two parts. The first part is the board definition, which tells the Arduino IDE (and PlatformIO) how to compile and flash the code to the MotorGo boards. The second part is the library to interface with the hardware on the MotorGo boards to use all of the features. Below, you will find instructions on how to install both parts.

## Board Definition
Since the MotorGo boards are based on the ESP32, you'll have to first install the ESP32 board definition.

**Arduino IDE 2.X**
* Go to `Tools > Board > Boards Manager...`
* Search for `ESP32`
* Install `esp32` by Espressif Systems

**Arduino IDE 1.X**
* Go to `Tools > Board > Boards Manager...`
* Search for `ESP32`
* Install `esp32` by Espressif Systems
*

**PlatformIO**
* Navigate to PlatformIO Home
* Go to `Platforms`
* Search for `Espressif 32`
* Click `Install`


Next, you'll have to install the MotorGo board definition, which is available in this respository.

**Arduino IDE 2.X**
* Go to `File > Preferences`
* Add `https://raw.githubusercontent.com/Every-Flavor-Robotics/motorgo-arduino/main/board_definitions/package_motorgo_index.json` to `Additional Boards Manager URLs`
* Go to `Tools > Board > Boards Manager...`
* Search for `MotorGo`
* Click `Install`

**Arduino IDE 1.X**
* Go to `File > Preferences`
* Add `https://raw.githubusercontent.com/Every-Flavor-Robotics/motorgo-arduino/main/board_definitions/package_motorgo_index.json` to `Additional Boards Manager URLs`
* Go to `Tools > Board > Boards Manager...`
* Search for `MotorGo`
* Click `Install`

**PlatformIO**
* Clone this respotory
* Navigate to board_definitions/
* Run the python script `python3 setup_platformio.py`


## Library
Below are the instructions for installing the MotorGo library. The source code  for the library is available [here](https://github.com/Every-Flavor-Robotics/motorgo-mini-driver.git).

**Arduino IDE 1.X/2.X**
* Download the latest release in `arduino_library/motorgo-mini-driver.zip`
* Go to `Sketch > Include Library > Add .ZIP Library...` and select the downloaded file


**PlatformIO**
* Add the following to your `platformio.ini` file:
```
lib_deps =
    https://github.com/Every-Flavor-Robotics/motorgo-mini-driver.git
``````

```
