
# Basic Servo setup
The servo motor will be used to control the rudder. In this section we will get the basic servo motor working with the ESP32. 

## Hardware
For this exercise you need the following parts:
1. ESP32 and usb cable to the pc
2. Servo motor (e.g., the MG90S 9g Metal Gear Micro Servo)
3. three wires (preferably red, black, and yellow)

Now for this exercise we are assuming you are using the Lonely Binary 3-Pack ESP32 Gold WiFi BT 3 Expansion Bases for Arduino IDE (see amazon: https://www.amazon.com/dp/B0FR3GVRD5). The type of wires will depend on the base extension board used or whether you are using a plain breadboard. 

## Step 1: Wire up the servo

The picture below shows an example setup using a bread board.
<p align="center">
  <img src="./images/esp32-to-servo.jpg" alt="servo connection via breadboard" width="400">
</p>

The servo has a three wire female plug with a brown, red, and orange wire.
1. connect your red wire to the ESP32 5V pin and the red servo wire
2. connect your black wire to the ESP32 grnd pin and the brown servo wire
3. connect your yellow wire to the ESP32 pin 18 and the yellow servo wire

## Step 2: attach any of the arms to the servo gear
There are three choices that come with the servo.

## Step 3: compile the basic_servo sketch to the ESP32

There is a program in the firmware directory called `basic_servo/basic_servo.ino`. This will step through various servo changes.

the first step is to compile:

```
cd ~/boat_drone/firmware
arduino-cli compile --fqbn esp32:esp32:esp32 ./basic_servo/basic_servo.ino 
```

Example output:

```
$ cd ~/boat_drone/firmware
$ arduino-cli compile --fqbn esp32:esp32:esp32 ./basic_servo/basic_servo.ino 
Sketch uses 284559 bytes (21%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22488 bytes (6%) of dynamic memory, leaving 305192 bytes for local variables. Maximum is 327680 bytes.
```


The second step is to upload. Once this is successful the servo arm will start moving through various positions.

```
cd ~/boat_drone/firmware
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./basic_servo
```

Example output.

```
$ arduino-cli compile --fqbn esp32:esp32:esp32 ./basic_servo/basic_servo.ino 
Sketch uses 284559 bytes (21%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22488 bytes (6%) of dynamic memory, leaving 305192 bytes for local variables. Maximum is 327680 bytes.
bueche@buecheutuntu2204:~/boat_drone_ws/boat_drone/firmware$ arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./basic_servo
esptool v5.3.1
Connected to ESP32 on /dev/ttyUSB0:
Chip type:          ESP32-D0WD-V3 (revision v3.1)
Features:           Wi-Fi, BT, Dual Core + LP Core, 240MHz, Vref calibration in eFuse, Coding Scheme None
Crystal frequency:  40MHz
MAC:                20:e7:c8:ab:2f:50

Stub flasher running.
Changing baud rate to 921600...
Changed.

Configuring flash size...

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.bootloader.bin' at 0x00001000...
Flash will be erased from 0x00001000 to 0x00007fff...
Wrote 24992 bytes (16001 compressed) at 0x00001000 in 0.4 seconds (530.1 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.partitions.bin' at 0x00008000...
Flash will be erased from 0x00008000 to 0x00008fff...
Wrote 3072 bytes (146 compressed) at 0x00008000 in 0.0 seconds (1029.9 kbit/s).
Hash of data verified.

Writing '/home/bueche/.arduino15/packages/esp32/hardware/esp32/3.3.11/tools/partitions/boot_app0.bin' at 0x0000e000...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.0 seconds (1331.2 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.bin' at 0x00010000...
Flash will be erased from 0x00010000 to 0x00055fff...
Wrote 284704 bytes (165569 compressed) at 0x00010000 in 2.2 seconds (1012.9 kbit/s).
Hash of data verified.

Hard resetting via RTS pin...
New upload port: /dev/ttyUSB0 (serial)
```

## Step 4: monitor the movement.

```
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

Example output.

```
```

